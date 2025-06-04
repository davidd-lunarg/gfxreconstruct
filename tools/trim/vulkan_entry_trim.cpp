/*
** Copyright (c) 2018-2023 Valve Corporation
** Copyright (c) 2018-2025 LunarG, Inc.
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

#include PROJECT_VERSION_HEADER_FILE

#include "tools/trim/vulkan_entry_trim.h"
#include "encode/vulkan_capture_manager.h"
#include "encode/vulkan_handle_wrapper_util.h"
#include "generated/generated_vulkan_api_call_encoders.h"
#include "generated/generated_vulkan_trim_func_table.h"
#include "util/platform.h"

#include "vulkan/vk_layer.h"

#include <array>
#include <cstring>
#include <mutex>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

GFXRECON_BEGIN_NAMESPACE(gfxrecon)
GFXRECON_BEGIN_NAMESPACE(vulkan_entry_trim)

VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL GetInstanceProcAddr(VkInstance instance, const char* pName)
{
    return VulkanEntryTrim::Get()->GetInstanceProcAddr(instance, pName);
}

VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL GetDeviceProcAddr(VkDevice device, const char* pName)
{
    return VulkanEntryTrim::Get()->GetDeviceProcAddr(device, pName);
}

VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL GetPhysicalDeviceProcAddr(VkInstance ourInstanceWrapper, const char* pName)
{
    return VulkanEntryTrim::Get()->GetPhysicalDeviceProcAddr(ourInstanceWrapper, pName);
}

VKAPI_ATTR VkResult VKAPI_CALL EnumerateDeviceExtensionProperties(VkPhysicalDevice       physicalDevice,
                                                                  const char*            pLayerName,
                                                                  uint32_t*              pPropertyCount,
                                                                  VkExtensionProperties* pProperties)
{
    return VulkanEntryTrim::Get()->EnumerateDeviceExtensionProperties(
        physicalDevice, pLayerName, pPropertyCount, pProperties);
}

VKAPI_ATTR VkResult VKAPI_CALL EnumerateInstanceExtensionProperties(const char*            pLayerName,
                                                                    uint32_t*              pPropertyCount,
                                                                    VkExtensionProperties* pProperties)
{
    return VulkanEntryTrim::Get()->EnumerateInstanceExtensionProperties(pLayerName, pPropertyCount, pProperties);
}

VKAPI_ATTR VkResult VKAPI_CALL EnumerateInstanceLayerProperties(uint32_t*          pPropertyCount,
                                                                VkLayerProperties* pProperties)
{
    return VulkanEntryTrim::Get()->EnumerateInstanceLayerProperties(pPropertyCount, pProperties);
}

VKAPI_ATTR VkResult VKAPI_CALL EnumerateDeviceLayerProperties(VkPhysicalDevice   physicalDevice,
                                                              uint32_t*          pPropertyCount,
                                                              VkLayerProperties* pProperties)
{
    return VulkanEntryTrim::Get()->EnumerateDeviceLayerProperties(physicalDevice, pPropertyCount, pProperties);
}

VKAPI_ATTR VkResult VKAPI_CALL dispatch_CreateInstance(const VkInstanceCreateInfo*  pCreateInfo,
                                                       const VkAllocationCallbacks* pAllocator,
                                                       VkInstance*                  pInstance)
{
    return VulkanEntryTrim::Get()->dispatch_CreateInstance(pCreateInfo, pAllocator, pInstance);
}

VKAPI_ATTR VkResult VKAPI_CALL dispatch_CreateDevice(VkPhysicalDevice             physicalDevice,
                                                     const VkDeviceCreateInfo*    pCreateInfo,
                                                     const VkAllocationCallbacks* pAllocator,
                                                     VkDevice*                    pDevice)
{
    return VulkanEntryTrim::Get()->dispatch_CreateDevice(physicalDevice, pCreateInfo, pAllocator, pDevice);
}

encode::VulkanEntryBase* VulkanEntryTrim::InitSingleton()
{
    return VulkanEntryBase::InitSingleton<VulkanEntryTrim>(GetVulkanFuncTableTrim());
}

VulkanEntryTrim::VulkanEntryTrim(const encode::VulkanFunctionTable& vulkan_function_table) :
    VulkanEntryBase(vulkan_function_table)
{
    InitializeLoader();
}

VulkanEntryTrim::~VulkanEntryTrim()
{
    ReleaseLoader();
}

VkResult VulkanEntryTrim::EnumerateDeviceExtensionProperties(VkPhysicalDevice       physicalDevice,
                                                             const char*            pLayerName,
                                                             uint32_t*              pPropertyCount,
                                                             VkExtensionProperties* pProperties)
{
    return VulkanEntryBase::EnumerateDeviceExtensionProperties(
        physicalDevice, pLayerName, pPropertyCount, pProperties, false);
}

VkResult VulkanEntryTrim::EnumerateInstanceExtensionProperties(const char*            pLayerName,
                                                               uint32_t*              pPropertyCount,
                                                               VkExtensionProperties* pProperties)
{
    VkResult result = VK_SUCCESS;

    if ((pLayerName != nullptr) && (util::platform::StringCompare(pLayerName, kLayerProps.layerName) == 0))
    {
        if (pPropertyCount != nullptr)
        {
            *pPropertyCount = 0;
        }
    }
    else if (pLayerName == nullptr)
    {
        // During trim, the GFXR capture code is not a layer registered with the loader, so forward the call to the
        // loader here.
        if (loader_handle_ != nullptr)
        {
            auto loader_enumerate_instance_extension_properties =
                reinterpret_cast<PFN_vkEnumerateInstanceExtensionProperties>(
                    util::platform::GetProcAddress(loader_handle_, "vkEnumerateInstanceExtensionProperties"));
            result = loader_enumerate_instance_extension_properties(pLayerName, pPropertyCount, pProperties);
        }
        else
        {
            result = VK_ERROR_INITIALIZATION_FAILED;
        }
    }
    else
    {
        result = VK_ERROR_LAYER_NOT_PRESENT;
    }

    return result;
}

// For the trim tool, this function is called by the capture manager handling for vkCreateInstance in
// VulkanCaptureManager::OverrideCreateInstance. It needs to create the actual (not wrapped) VkInstance object.
VKAPI_ATTR VkResult VKAPI_CALL VulkanEntryTrim::dispatch_CreateInstance(const VkInstanceCreateInfo*  pCreateInfo,
                                                                        const VkAllocationCallbacks* pAllocator,
                                                                        VkInstance*                  pInstance)
{
    VkResult result = VK_ERROR_INITIALIZATION_FAILED;

    if (loader_handle_ != nullptr)
    {
        auto loader_get_instance_proc_addr = reinterpret_cast<PFN_vkGetInstanceProcAddr>(
            util::platform::GetProcAddress(loader_handle_, "vkGetInstanceProcAddr"));

        if (loader_get_instance_proc_addr != nullptr)
        {
            auto loader_create_instance_function =
                reinterpret_cast<PFN_vkCreateInstance>(loader_get_instance_proc_addr(nullptr, "vkCreateInstance"));

            if (loader_create_instance_function != nullptr)
            {
                // Make the Vulkan call to create the instance.
                result = loader_create_instance_function(pCreateInfo, pAllocator, pInstance);

                if ((result == VK_SUCCESS) && pInstance && (*pInstance != nullptr))
                {
                    AddInstanceHandle(*pInstance);
                    VkInstance unwrapped_instance = *pInstance;

                    encode::VulkanCaptureManager* manager = encode::VulkanCaptureManager::Get();
                    assert(manager != nullptr);

                    // Initialize the capture manager with the Vulkan loader's vkGetInstanceProcAddr so the capture
                    // manager is calling Vulkan functions directly.
                    manager->InitVkInstance(pInstance, loader_get_instance_proc_addr);

                    // Register the next layer's GetPhysicalDeviceProcAddr func only after *pInstance
                    // has been updated to our wrapper in manager->InitVkInstance() above:
                    auto fpNextGetPhysicalDeviceProcAddr = reinterpret_cast<PFN_GetPhysicalDeviceProcAddr>(
                        loader_get_instance_proc_addr(unwrapped_instance, "vk_layerGetPhysicalDeviceProcAddr"));
                    SetInstanceNextGPDPA(*pInstance, fpNextGetPhysicalDeviceProcAddr);
                }
            }
        }
    }

    return result;
}

// For the trim tool, this function is called by the capture manager handling for vkCreateDevice in
// VulkanCaptureManager::OverrideCreateDevice. It needs to create the actual (not wrapped) VkDevice object.
VKAPI_ATTR VkResult VKAPI_CALL VulkanEntryTrim::dispatch_CreateDevice(VkPhysicalDevice             physicalDevice,
                                                                      const VkDeviceCreateInfo*    pCreateInfo,
                                                                      const VkAllocationCallbacks* pAllocator,
                                                                      VkDevice*                    pDevice)
{
    VkResult result = VK_ERROR_INITIALIZATION_FAILED;

    VkInstance layer_instance = GetInstanceHandle(physicalDevice);

    if (loader_handle_ != nullptr)
    {
        auto loader_get_instance_proc_addr = reinterpret_cast<PFN_vkGetInstanceProcAddr>(
            util::platform::GetProcAddress(loader_handle_, "vkGetInstanceProcAddr"));

        PFN_vkCreateDevice loader_create_device_function =
            reinterpret_cast<PFN_vkCreateDevice>(loader_get_instance_proc_addr(layer_instance, "vkCreateDevice"));

        if (loader_create_device_function)
        {
            result = loader_create_device_function(physicalDevice, pCreateInfo, pAllocator, pDevice);

            if ((result == VK_SUCCESS) && pDevice && (*pDevice != nullptr))
            {
                auto get_device_proc_addr = reinterpret_cast<PFN_vkGetDeviceProcAddr>(
                    loader_get_instance_proc_addr(layer_instance, "vkGetDeviceProcAddr"));

                encode::VulkanCaptureManager* manager = encode::VulkanCaptureManager::Get();
                assert(manager != nullptr);
                manager->InitVkDevice(pDevice, get_device_proc_addr);
            }
        }
    }

    return result;
}

void VulkanEntryTrim::InitializeLoader()
{
    loader_handle_ = graphics::InitializeLoader();
    GFXRECON_ASSERT(loader_handle_ != nullptr);
}

void VulkanEntryTrim::ReleaseLoader()
{
    if (loader_handle_)
    {
        graphics::ReleaseLoader(loader_handle_);
    }
}

GFXRECON_END_NAMESPACE(vulkan_entry_trim)
GFXRECON_END_NAMESPACE(gfxrecon)
