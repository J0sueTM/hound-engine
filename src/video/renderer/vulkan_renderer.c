/**
 * @file src/video/renderer/vulkan_renderer.c
 * @author Josue Teodoro Moreira <teodoro.josue@protonmail.ch>
 * @date August 08, 2021
 *
 * Copyright (C) 2021 Josue Teodoro Moreira
 *  
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *  
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * GNU General Public License for more details.
 */

#include "renderer.h"

#ifdef HND_DEBUG
static VkResult
hnd_create_vulkan_debug_utils_messenger_ext
(
  VkInstance                                _instance,
  const VkDebugUtilsMessengerCreateInfoEXT *_create_info,
  const VkAllocationCallbacks              *_allocator,
  VkDebugUtilsMessengerEXT                 *_messenger)
{
  PFN_vkCreateDebugUtilsMessengerEXT create_debug_utils_messenger_ext_function =
    (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(_instance, "vkCreateDebugUtilsMessengerEXT");
  
  if (!create_debug_utils_messenger_ext_function)
    return VK_ERROR_EXTENSION_NOT_PRESENT;

  return create_debug_utils_messenger_ext_function(_instance, _create_info, _allocator, _messenger);
}

static void
hnd_destroy_vulkan_debug_utils_messenger_ext
(
  VkInstance                   _instance,
  VkDebugUtilsMessengerEXT     _messenger,
  const VkAllocationCallbacks *_allocator
)
{
  PFN_vkDestroyDebugUtilsMessengerEXT destroy_debug_utils_messenger_ext_function =
    (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(_instance, "vkDestroyDebugutilsmessengerEXT");

  if (!destroy_debug_utils_messenger_ext_function)
    return;

  destroy_debug_utils_messenger_ext_function(_instance, _messenger, _allocator);
}
#endif /* HND_DEBUG */

static int
hnd_check_physical_device_queue_family_properties
(
  VkPhysicalDevice *_physical_device
)
{
  uint32_t queue_family_properties_count;
  vkGetPhysicalDeviceQueueFamilyProperties(*_physical_device, &queue_family_properties_count, NULL);
  if (!hnd_assert(queue_family_properties_count > 0, "Not enough queue family properties"))
    return HND_NK;

  VkQueueFamilyProperties queue_family_properties[queue_family_properties_count];
  vkGetPhysicalDeviceQueueFamilyProperties(*_physical_device, &queue_family_properties_count, queue_family_properties);

  /* Check for queue graphics bit support. */
  int is_graphics_bit_supported = HND_NK;
  for (uint32_t i = 0; i < queue_family_properties_count; ++i)
  {
    if ((queue_family_properties + i)->queueFlags & VK_QUEUE_GRAPHICS_BIT)
    {
      is_graphics_bit_supported = HND_OK;

      goto end_queue_family_properties_check;
    }
  }

end_queue_family_properties_check:
  return is_graphics_bit_supported;
}

static void
hnd_pick_best_physical_device
(
  hnd_renderer_t   *_renderer,
  VkPhysicalDevice *_physical_devices
)
{
  /* Don't need to do extra work if there're no physical devices other than the default. */
  VkPhysicalDevice *best_physical_device = _physical_devices;
  if (_renderer->physical_device_count == 1)
    goto skip_physical_device_selection;
  
  VkPhysicalDevice *current_physical_device;
  VkPhysicalDeviceProperties current_physical_device_properties;
  VkPhysicalDeviceFeatures current_physical_device_features;
  
  int best_physical_device_score = 0;
  int current_physical_device_score = 0;

  /* Since we already assigned the best physical device to the first available physical device,
   * we need to begin from the second position.
   */
  for (uint32_t i = 1; i < _renderer->physical_device_count; ++i)
  {
    current_physical_device = (_physical_devices + i);

    /* Get physical device data */
    vkGetPhysicalDeviceProperties(*current_physical_device, &current_physical_device_properties);
    vkGetPhysicalDeviceFeatures(*current_physical_device, &current_physical_device_features);
    
    current_physical_device_score = 0;

    if (!hnd_check_physical_device_queue_family_properties(current_physical_device) ||
        !current_physical_device_features.geometryShader)
      goto skip_physical_device_score_ranking;

    /* GPU type */
    switch (current_physical_device_properties.deviceType)
    {
    case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
      current_physical_device_score += 10;
      
      break;
    case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
      current_physical_device_score += 5;
      
      break;
    case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
      current_physical_device_score += 2;
      
      break;
    case VK_PHYSICAL_DEVICE_TYPE_CPU:
      current_physical_device_score += 1;

      break;
    default:
      current_physical_device_score += 0;
 
      break;
    }

    current_physical_device_score += current_physical_device_properties.limits.maxImageDimension2D;

skip_physical_device_score_ranking:
    if (current_physical_device_score > best_physical_device_score)
    {
      best_physical_device = current_physical_device;
      best_physical_device_score = current_physical_device_score;
    }
  }

skip_physical_device_selection:
  _renderer->physical_device = *best_physical_device;
}

int
hnd_init_renderer
(
  hnd_renderer_t *_renderer
)
{
  if (!hnd_assert(_renderer != NULL, HND_SYNTAX))
    return HND_NK;

#ifdef HND_DEBUG
  hnd_check_validation_layers_support(_renderer);
#else
  _renderer->are_validation_layers_supported = HND_NK;
#endif /* HND_DEBUG */

  if (!hnd_create_instance(_renderer))
    return HND_NK;
  if (!hnd_get_physical_devices(_renderer))
    return HND_NK;
  
  return HND_OK;
}

void
hnd_end_renderer
(
  hnd_renderer_t *_renderer
)
{
#ifdef HND_DEBUG
  if (_renderer->are_validation_layers_supported)
    hnd_destroy_vulkan_debug_utils_messenger_ext(_renderer->instance, _renderer->messenger, NULL);
#endif /* HND_DEBUG */
  
  vkDestroyInstance(_renderer->instance, NULL);
}

#ifdef HND_DEBUG
void
hnd_check_validation_layers_support
(
  hnd_vulkan_renderer_t *_renderer
)
{
  _renderer->validation_layers[0] = "VK_LAYER_KHRONOS_validation";
  
  vkEnumerateInstanceLayerProperties(&_renderer->supported_validation_layer_count, NULL);
  if (_renderer->supported_validation_layer_count < 1)
    return;

  VkLayerProperties supported_validation_layers[_renderer->supported_validation_layer_count];
  vkEnumerateInstanceLayerProperties(&_renderer->supported_validation_layer_count,
                                     supported_validation_layers);

  int is_current_validation_layer_supported = HND_NK;
  for (uint32_t i = 0; i < HND_VALIDATION_LAYER_COUNT; ++i)
  {
    for (uint32_t j = 0; j < _renderer->supported_validation_layer_count; ++j)
    {
      if (!strcmp(_renderer->validation_layers[i], supported_validation_layers[j].layerName))
        is_current_validation_layer_supported = HND_OK;
    }

    if (!is_current_validation_layer_supported)
    {
      hnd_print_debug(HND_WARNING, "Incomplete vulkan validation layer support", HND_FAILURE);
      _renderer->are_validation_layers_supported = HND_NK;

      return;
    }

    is_current_validation_layer_supported = HND_NK;
  }
  
  _renderer->are_validation_layers_supported = HND_OK;
}

void
hnd_create_debug_messenger
(
  hnd_vulkan_renderer_t *_renderer
)
{
  _renderer->messenger_create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
  _renderer->messenger_create_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                                                     VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                                     VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
  _renderer->messenger_create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT    |
                                                 VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                                                 VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
  _renderer->messenger_create_info.pfnUserCallback = vulkan_debug_callback;
  _renderer->messenger_create_info.pUserData = NULL;

  VkResult messenger_creation_result = hnd_create_vulkan_debug_utils_messenger_ext(_renderer->instance,
                                                                                   &_renderer->messenger_create_info,
                                                                                   NULL,
                                                                                   &_renderer->messenger);
  if (messenger_creation_result != VK_SUCCESS)
    hnd_print_debug(HND_ERROR, "Could not create debug messenger", HND_FAILURE);
}
#endif /* HND_DEBUG */

int
hnd_check_instance_extensions_support
(
  hnd_vulkan_renderer_t *_renderer
)
{
  uint32_t supported_extension_count;
  vkEnumerateInstanceExtensionProperties(NULL, &supported_extension_count, NULL);

  VkExtensionProperties supported_extensions[supported_extension_count];
  vkEnumerateInstanceExtensionProperties(NULL, &supported_extension_count, supported_extensions);

  int is_current_extension_supported = HND_NK;
  int are_all_extensions_supported = HND_OK;
  for (uint32_t i = 0; i < HND_INSTANCE_EXTENSION_COUNT; ++i)
  {
    for (uint32_t j = 0; j < supported_extension_count; ++j)
    {
      if (!strcmp(_renderer->instance_extensions[i], supported_extensions[j].extensionName))
        is_current_extension_supported = HND_OK;
    }

    if (!is_current_extension_supported)
    {
      hnd_print_debug(HND_WARNING, "Incomplete vulkan instance extension support:", HND_FAILURE);
      hnd_print_debug("          ", _renderer->instance_extensions[i], HND_FAILURE);

      are_all_extensions_supported = HND_NK;
    }
      
    is_current_extension_supported = HND_NK;
  }

  if (are_all_extensions_supported)
    hnd_print_debug(HND_LOG, "All instance extensions are supported", HND_SUCCESS);
  
  return are_all_extensions_supported;
}

int
hnd_create_instance
(
  hnd_vulkan_renderer_t *_renderer
)
{
  if (!hnd_assert(_renderer != NULL, HND_SYNTAX))
    return HND_NK;

  /* Instance info */
  _renderer->application_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  _renderer->application_info.pNext = NULL;
  _renderer->application_info.pApplicationName = HND_PROJECT_NAME;
  _renderer->application_info.applicationVersion = VK_MAKE_VERSION(HND_PROJECT_VERSION_MAJOR, 
                                                                   HND_PROJECT_VERSION_MINOR,
                                                                   HND_PROJECT_VERSION_PATCH);
  _renderer->application_info.pEngineName = HND_NAME;
  _renderer->application_info.engineVersion = VK_MAKE_VERSION(HND_VERSION_MAJOR,
                                                              HND_VERSION_MINOR,
                                                              HND_VERSION_PATCH);
  _renderer->application_info.apiVersion = VK_API_VERSION_1_2;

  /* Instance creation info */
  _renderer->instance_create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  _renderer->instance_create_info.pNext = NULL;
  _renderer->instance_create_info.flags = 0;
  _renderer->instance_create_info.pApplicationInfo = &_renderer->application_info;

  /* Instance extensions */
  _renderer->instance_create_info.enabledExtensionCount = HND_INSTANCE_EXTENSION_COUNT;
  _renderer->instance_extensions[0] = VK_KHR_SURFACE_EXTENSION_NAME;
  _renderer->instance_extensions[1] = VK_KHR_XCB_SURFACE_EXTENSION_NAME;

#ifdef HND_DEBUG
  if (_renderer->are_validation_layers_supported)
  {
    _renderer->instance_create_info.enabledLayerCount = HND_VALIDATION_LAYER_COUNT;
    _renderer->instance_create_info.ppEnabledLayerNames = (const char *const *)_renderer->validation_layers;
  }

  _renderer->instance_extensions[2] = VK_EXT_DEBUG_UTILS_EXTENSION_NAME;
#else
  _renderer->instance_create_info.enabledLayerCount = 0;
#endif /* HND_DEBUG */
  
  _renderer->instance_create_info.ppEnabledExtensionNames = (const char *const *)_renderer->instance_extensions;

  if (!hnd_check_instance_extensions_support(_renderer))
    return HND_NK;

  /* Instance creation */
  VkResult instance_creation_result = vkCreateInstance(&_renderer->instance_create_info,
                                                       NULL,
                                                       &_renderer->instance);
  if (!hnd_assert(instance_creation_result != VK_ERROR_INCOMPATIBLE_DRIVER,
                 "Could not find a compatible vulkan ICD."
                 " Please make sure your drivers supports vulkan"))
    return HND_NK;
  else if (!hnd_assert(instance_creation_result == VK_SUCCESS,
           "Could not find a vulkan ICD."
           " Make sure your drivers has an ICD"))
    return HND_NK;

  hnd_print_debug(HND_LOG, "Created vulkan instance", HND_SUCCESS);
  return HND_OK;
}

int
hnd_get_physical_devices
(
  hnd_vulkan_renderer_t *_renderer
)
{
  if (!hnd_assert(_renderer != NULL, HND_SYNTAX))
    return HND_NK;

  vkEnumeratePhysicalDevices(_renderer->instance, &_renderer->physical_device_count, NULL);
  if (!hnd_assert(_renderer->physical_device_count > 0,
                  "Could not find GPUs with Vulkan physical devices (GPUs with Vulkan support)"))
    return HND_NK;

  VkPhysicalDevice physical_devices[_renderer->physical_device_count];
  vkEnumeratePhysicalDevices(_renderer->instance, &_renderer->physical_device_count,
                             physical_devices);
  
#ifdef HND_PICK_PHYSICAL_DEVICE
  if (hnd_assert(HND_PICK_PHYSICAL_DEVICE >= 0 &&
                 HND_PICK_PHYSICAL_DEVICE < physical_device_count,
                 "Given HND_PICK_PHYSICAL_DEVICE is out of bound. Ignoring"))
  {
    _renderer->physical_device = *(_renderer->available_physical_devices + HND_PICK_PHYSICAL_DEVICE);
    
    return HND_OK;
  }
#endif /* HND_PICK_PHYSICAL_DEVICE */

  hnd_pick_best_physical_device(_renderer, physical_devices);
  
  return HND_OK;
}

void
hnd_clear_render
(
  float _red,
  float _green,
  float _blue,
  float _alpha
)
{
  /* IMPLEMENT ME(JsueTM) */
}

void
hnd_swap_renderer_buffers
(
  hnd_renderer_t *_renderer
)
{
  /* IMPLEMENT ME(JsueTM) */
}
