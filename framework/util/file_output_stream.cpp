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

#include "util/file_output_stream.h"

#include "util/logging.h"
#include "util/platform.h"

#include <shared_mutex>

GFXRECON_BEGIN_NAMESPACE(gfxrecon)
GFXRECON_BEGIN_NAMESPACE(util)

const size_t kBufferCount = 4;

FileOutputStream::FileOutputStream(const std::string& filename, size_t buffer_size, bool append) :
    write_thread_done_(false), buffer_size_(64 * 1024 * 1024), current_buffer_(nullptr), current_buffer_index_(0),
    file_(nullptr), own_file_(true)
{
    const char* mode   = append ? "ab" : "wb";
    int32_t     result = platform::FileOpen(&file_, filename.c_str(), mode);

    for (size_t i = 0; i < kBufferCount; ++i)
    {
        FileBuffer* new_buffer = new FileBuffer();
        new_buffer->data.resize(buffer_size_);
        new_buffer->position = 0;
        buffers_[i] = new_buffer;
    }
    current_buffer_ = buffers_[current_buffer_index_];

    if (file_ != nullptr)
    {
        /*result = platform::SetFileBufferSize(file_, buffer_size_);
        if (result != 0)
        {
            GFXRECON_LOG_WARNING("Failed to set file buffer size. File writing performance may be affected.");
        }*/
    }
    else
    {
        GFXRECON_LOG_ERROR("fopen(%s, %s) failed (errno = %d)", filename.c_str(), mode, result);
    }

    write_thread_ = std::thread(&FileOutputStream::FileWriteWorker, this);
}

FileOutputStream::FileOutputStream(FILE* file, bool owned) : file_(file), own_file_(owned), buffer_size_(0)
{
    // TODO support this constructor with buffers or remove it
    assert(false);
}

FileOutputStream::~FileOutputStream()
{
    write_thread_done_ = true;
    write_thread_.join();
    if ((file_ != nullptr) && own_file_)
    {
        platform::FileClose(file_);
    }
    for (FileBuffer* buffer : buffers_)
    {
        delete buffer;
    }
}

size_t FileOutputStream::Write(const void* data, size_t len)
{
    // TODO clean this up
    if (len > buffer_size_)
    {
        shared_mutex_.lock_shared();
        need_buffer_swap_.store(true);
        shared_mutex_.unlock_shared();

        // Wait until buffer swap has completed.
        while (need_buffer_swap_.load())
        {}

        shared_mutex_.lock();
        platform::FileWrite(data, 1, len, file_);
        shared_mutex_.unlock();

        GFXRECON_LOG_WARNING("Write data of len %" PRIu64, len);
        return 0;
    }

    shared_mutex_.lock_shared();

    // Reserve bytes in the current buffer.
    FileBuffer* write_buffer = current_buffer_;
    size_t      write_pos    = write_buffer->position.fetch_add(len);

    // Determine if a buffer swap is needed.
    while ((write_pos + len) > buffer_size_)
    {
        need_buffer_swap_.store(true);

        // Indicate to old buffer not to write the reserved bytes.
        write_buffer->position.fetch_sub(len);

        // Allow write thread to acquire unique lock to change buffer.
        shared_mutex_.unlock_shared();

        // Wait until buffer swap has completed.
        while (need_buffer_swap_.load())
        {}

        // Try again.
        shared_mutex_.lock_shared();

        // Get new buffer and reserve bytes.
        write_buffer = current_buffer_;
        write_pos    = write_buffer->position.fetch_add(len);
    }

    util::platform::MemoryCopy(write_buffer->data.data() + write_pos, len, data, len);

    shared_mutex_.unlock_shared();

    return len;
}

GFXRECON_END_NAMESPACE(util)
GFXRECON_END_NAMESPACE(gfxrecon)
