/*
** Copyright (c) 2018 Valve Corporation
** Copyright (c) 2018 LunarG, Inc.
**
** Permission is hereby granted, free of charge, to any person obtaining a
** copy of this software and associated documentation files (the "Software"),
** to deal in the Software without restriction, including without limitation
** the rights to use, copy, modify, merge, publish, distribute, sublicense,
** and/or sell copies of the Software, and to permit persons to whom the
** Software is furnished to do so, subject to the following conditions:
**
** The above copyright notice and this permission notice shall be included in
** all copies or substantial portions of the Software.
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
** FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
** DEALINGS IN THE SOFTWARE.
*/

#ifndef GFXRECON_UTIL_FILE_OUTPUT_STREAM_H
#define GFXRECON_UTIL_FILE_OUTPUT_STREAM_H

#include "util/defines.h"
#include "util/logging.h"
#include "util/output_stream.h"
#include "util/platform.h"

#include <array>
#include <cstdio>
#include <inttypes.h>
#include <mutex>
#include <string>

GFXRECON_BEGIN_NAMESPACE(gfxrecon)
GFXRECON_BEGIN_NAMESPACE(util)

class FileOutputStream : public OutputStream
{
  public:
    // buffer_size controls the size of file stream buffer. If buffer_size is 0, file writes will be unbuffered.
    FileOutputStream(const std::string& filename, size_t buffer_size, bool append = false);

    FileOutputStream(FILE* file, bool owned = false);

    virtual ~FileOutputStream() override;

    virtual bool IsValid() override { return (file_ != nullptr); }

    virtual size_t Write(const void* data, size_t len) override;

    virtual void Flush() override { platform::FileFlush(file_); }

  private:
    FILE* file_;
    bool  own_file_;

    class SharedMutex
    {
      public:
        SharedMutex() : reader_count_(0), wait_for_writer_(false) {}

        void lock()
        {
            writer_mutex.lock();
            wait_for_writer_.store(true);

            while (reader_count_.load() > 0)
            {}
        }

        void unlock()
        {
            writer_mutex.unlock();
            wait_for_writer_.store(false);
        }

        void lock_shared()
        {
            reader_count_.fetch_add(1);
            if (wait_for_writer_.load())
            {
                reader_count_.fetch_sub(1);
                writer_mutex.lock();
                reader_count_.fetch_add(1);
                writer_mutex.unlock();
            }
        }

        void unlock_shared() { reader_count_.fetch_sub(1); }

      private:
        std::mutex         writer_mutex;
        std::atomic_size_t reader_count_;
        std::atomic_bool   wait_for_writer_;
    };

    SharedMutex shared_mutex_;

    struct FileBuffer
    {
        std::vector<uint8_t> data;
        std::atomic_size_t   position;
    };

    std::atomic_bool need_flush_{ false };
    std::atomic_bool need_buffer_swap_{ false };

    void FileWriteWorker()
    {
        const size_t kWriteInterval = 1024 * 1024;
        size_t       previous_write = 0;

        while (!write_thread_done_)
        {
            // Check if a buffer write or swap needs to happen.
            size_t write_pos  = current_buffer_->position.load();
            size_t write_size = write_pos - previous_write;
            if (write_size > kWriteInterval || need_buffer_swap_.load())
            {
                // Acquire unique lock to ensure all pending memory copies to addresses below write_pos are done.
                shared_mutex_.lock();

                // Get actual bytes now that main threads are waiting.
                write_pos  = current_buffer_->position.load();
                write_size = write_pos - previous_write;

                if (need_buffer_swap_.load())
                {
                    uint8_t* final_write_ptr = current_buffer_->data.data() + previous_write;
                    SwapBuffers();
                    previous_write = 0;
                    need_buffer_swap_.store(false);

                    // Buffer swap complete, allow main threads to continue;
                    shared_mutex_.unlock();

                    // Write remaining bytes to completed buffer.
                    platform::FileWrite(final_write_ptr, 1, write_size, file_);
                }
                else
                {
                    // For normal operation, the main threads can continue while the prepared data is written to file.
                    shared_mutex_.unlock();

                    platform::FileWrite(current_buffer_->data.data() + previous_write, 1, write_size, file_);
                    previous_write = write_pos;
                }
            }
            std::this_thread::yield();
        }

        // write remainder of buffer to disk
        platform::FileWrite(
            current_buffer_->data.data() + previous_write, 1, current_buffer_->position.load() - previous_write, file_);
    }

    void SwapBuffers()
    {
        ++current_buffer_index_;
        current_buffer_index_ = current_buffer_index_ % kBufferCount;
        current_buffer_       = buffers_[current_buffer_index_];
        current_buffer_->position.store(0);
    }

    bool write_thread_done_;

    static const size_t kBufferCount = 2;

    const size_t                          buffer_size_;
    std::array<FileBuffer*, kBufferCount> buffers_;
    FileBuffer*                           current_buffer_;
    size_t                                current_buffer_index_;

    std::thread write_thread_;
};

GFXRECON_END_NAMESPACE(util)
GFXRECON_END_NAMESPACE(gfxrecon)

#endif // GFXRECON_UTIL_FILE_OUTPUT_STREAM_H
