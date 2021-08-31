/**
 * @file src/video/renderer/vulkan/common_vulkan_renderer.c
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

#include "../renderer.h"

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

static void
hnd_pick_best_physical_device
(
  hnd_vulkan_renderer_t   *_renderer,
  VkPhysicalDevice *_physical_devices
)
{
  VkPhysicalDevice *best_physical_device = _physical_devices;
  VkPhysicalDevice *current_physical_device;

  VkPhysicalDeviceProperties current_physical_device_properties;
  VkPhysicalDeviceFeatures current_physical_device_features;
  
  int best_physical_device_score = 0;
  int current_physical_device_score = 0;
  
  uint32_t best_physical_device_queue_family_with_graphics_bit_index = 0;
  uint32_t current_physical_device_queue_family_with_graphics_bit_index = 0;

  uint32_t current_physical_device_queue_family_properties_count;
  VkQueueFamilyProperties current_physical_device_queue_family_properties[HND_MAX_QUEUE_FAMILY_PROPERTIES];
  
  for (uint32_t i = 0; i < _renderer->physical_device_count; ++i)
  {
    current_physical_device = (_physical_devices + i);
    current_physical_device_score = 0;

    /* Get physical device data */
    vkGetPhysicalDeviceProperties(*current_physical_device, &current_physical_device_properties);
    vkGetPhysicalDeviceFeatures(*current_physical_device, &current_physical_device_features);

    /**
     * @note This goes before checking geometry shader support since the physical device's
     * queue family with graphics bit may not be the same as the best ranked in this loop.
     */
    vkGetPhysicalDeviceQueueFamilyProperties(*current_physical_device,
                                             &current_physical_device_queue_family_properties_count,
                                             NULL);
    if (!hnd_assert(current_physical_device_queue_family_properties_count > 0, "Not enough queue family properties"))
      goto skip_physical_device_score_ranking;

    memset(current_physical_device_queue_family_properties,
           0x00,
           HND_MAX_QUEUE_FAMILY_PROPERTIES * sizeof(VkQueueFamilyProperties));
    vkGetPhysicalDeviceQueueFamilyProperties(*current_physical_device,
                                             &current_physical_device_queue_family_properties_count,
                                             current_physical_device_queue_family_properties);
   
    for (uint32_t i = 0; i < current_physical_device_queue_family_properties_count; ++i)
    {
      if (current_physical_device_queue_family_properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
      {
        current_physical_device_queue_family_with_graphics_bit_index = i;

        hnd_print_debug(HND_LOG, "Found queue family with graphics bit", HND_SUCCESS);
#ifdef HND_DEBUG
        printf("       Code: %d\n", i);
#endif /* HND_DEBUG */

        break;
      }
    }
    
    if (!hnd_check_physical_device_extension_support(_renderer, current_physical_device))
      goto skip_physical_device_score_ranking;
    if (!current_physical_device_queue_family_with_graphics_bit_index)
      goto skip_physical_device_score_ranking;
    if (!current_physical_device_features.geometryShader)
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
      best_physical_device_score = current_physical_device_score;
      best_physical_device_queue_family_with_graphics_bit_index =
        current_physical_device_queue_family_with_graphics_bit_index;
      
      best_physical_device = current_physical_device;
    }
  }
  
  _renderer->physical_device = *best_physical_device;
  _renderer->physical_device_queue_family_with_graphics_bit_index =
    best_physical_device_queue_family_with_graphics_bit_index;
}

static void
hnd_get_swapchain_images
(
  hnd_vulkan_renderer_t *_renderer
)
{
  memset(_renderer->swapchain_images, 0x00, HND_MAX_SWAPCHAIN_IMAGES * sizeof(VkImage));

  vkGetSwapchainImagesKHR(_renderer->logical_device,
                          _renderer->swapchain,
                          &_renderer->swapchain_image_count,
                          _renderer->swapchain_images);
}

int
hnd_init_renderer
(
  hnd_vulkan_renderer_t *_renderer
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
  if (!hnd_create_logical_device(_renderer))
    return HND_NK;
  
  return HND_OK;
}

void
hnd_end_renderer
(
  hnd_vulkan_renderer_t *_renderer
)
{
#ifdef HND_DEBUG
  if (_renderer->are_validation_layers_supported)
    hnd_destroy_vulkan_debug_utils_messenger_ext(_renderer->instance, _renderer->messenger, NULL);
#endif /* HND_DEBUG */

  for (uint32_t i = 0; i < _renderer->swapchain_image_count; ++i)
    vkDestroyImageView(_renderer->logical_device, _renderer->swapchain_image_views[i], NULL);
    
  vkDestroySwapchainKHR(_renderer->logical_device, _renderer->swapchain, NULL);
  vkDestroySurfaceKHR(_renderer->instance, _renderer->surface, NULL);
  vkDestroyDevice(_renderer->logical_device, NULL);
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

  hnd_print_debug(HND_LOG, HND_CREATED("debug messenger"), HND_SUCCESS);
}
#endif /* HND_DEBUG */

/**
 * @brief Checks if extensions in hnd_vulkan_renderer_t->instance_extensions
 *        are supported and can be enabled.
 *
 * @param _renderer Specifies the renderer containing the instance extensions.
 *
 * @return Function state. HND_OK or HND_NK.
 */
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

  /**
   * @note If we're looking for a specific extension, we loop through supported_extensions until
   * we whether find an occurence or reach the end. Otherwise, if we're looking for all extensions support,
   * we'll loop through both required extensions and available.
   */
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
#ifdef HND_WIN23
  _renderer->instance_extensions[1] = VK_KHR_WIN32_SURFACE_EXTENSION_NAME;
#else
  _renderer->instance_extensions[1] = VK_KHR_XCB_SURFACE_EXTENSION_NAME;
#endif /* HND_WIN32 */

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
hnd_check_physical_device_extension_support
(
  hnd_vulkan_renderer_t *_renderer,
  VkPhysicalDevice      *_device
)
{
  uint32_t supported_extension_count;
  vkEnumerateDeviceExtensionProperties(*_device, NULL, &supported_extension_count, NULL);
  if (!hnd_assert(supported_extension_count > 0, "Not enough supported physical device extensions"))
    return HND_NK;

  VkExtensionProperties supported_extensions[supported_extension_count];
  vkEnumerateDeviceExtensionProperties(*_device, NULL, &supported_extension_count, supported_extensions);

  int is_current_extension_supported = HND_NK;
  int are_all_extensions_supported = HND_OK;
  for (uint32_t i = 0; i < HND_PHYSICAL_DEVICE_EXTENSION_COUNT; ++i)
  {
    for (uint32_t j = 0; j < supported_extension_count; ++j)
    {
      if (!strcmp(_renderer->physical_device_extensions[i], supported_extensions[j].extensionName))
        is_current_extension_supported = HND_OK;
    }

    if (!hnd_assert(is_current_extension_supported, "Incomplete physical device extensions support"))
    {
      printf("      %s", _renderer->physical_device_extensions[i]);
      
      are_all_extensions_supported = HND_NK;
      goto skip_physical_device_extension_support_check;
    }
    
    is_current_extension_supported = HND_NK;
  }

  hnd_print_debug(HND_LOG, "All physical device extensions are supported", HND_SUCCESS);
skip_physical_device_extension_support_check:
  return are_all_extensions_supported;
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

  _renderer->physical_device_extensions[0] = VK_KHR_SWAPCHAIN_EXTENSION_NAME;
  hnd_pick_best_physical_device(_renderer, physical_devices);

  hnd_print_debug(HND_LOG, HND_CREATED("physical device"), HND_SUCCESS);
  return HND_OK;
}

int
hnd_create_logical_device
(
  hnd_vulkan_renderer_t *_renderer
)
{
  if (!hnd_assert(_renderer != NULL, HND_SYNTAX))
    return HND_NK;

  float logical_device_queue_create_info_priority = 1.0f;
  _renderer->logical_device_queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
  _renderer->logical_device_queue_create_info.queueCount = 1;
  _renderer->logical_device_queue_create_info.pQueuePriorities = &logical_device_queue_create_info_priority;
  _renderer->logical_device_queue_create_info.queueFamilyIndex =
    _renderer->physical_device_queue_family_with_graphics_bit_index;

  _renderer->logical_device_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  _renderer->logical_device_create_info.pQueueCreateInfos = &_renderer->logical_device_queue_create_info;
  _renderer->logical_device_create_info.queueCreateInfoCount = 1;
  _renderer->logical_device_create_info.pEnabledFeatures = &_renderer->physical_device_features;
  _renderer->logical_device_create_info.enabledLayerCount = 0;
  _renderer->logical_device_create_info.enabledExtensionCount = HND_PHYSICAL_DEVICE_EXTENSION_COUNT;
  _renderer->logical_device_create_info.ppEnabledExtensionNames =
    (const char *const *)_renderer->physical_device_extensions;
  
  VkResult logical_device_creation_result = vkCreateDevice(_renderer->physical_device,
                                                           &_renderer->logical_device_create_info,
                                                           NULL,
                                                           &_renderer->logical_device);
  if (!hnd_assert(logical_device_creation_result == VK_SUCCESS, "Could not create logical device"))
  {
#ifdef HND_DEBUG
    printf("         Code: %d\n", logical_device_creation_result);
#endif /* HND_DEBUG */
    
    return HND_NK;
  }

  vkGetDeviceQueue(_renderer->logical_device,
                   _renderer->physical_device_queue_family_with_graphics_bit_index,
                   0,
                   &_renderer->logical_device_graphics_queue);

  hnd_print_debug(HND_LOG, HND_CREATED("logical device"), HND_SUCCESS);
  return HND_OK;
}

int
hnd_create_swapchain
(
  hnd_vulkan_renderer_t *_renderer
)
{
  if (!hnd_assert(_renderer != NULL, HND_SYNTAX))
    return HND_NK;

  VkResult surface_capabilities_result =
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(_renderer->physical_device,
                                              _renderer->surface,
                                              &_renderer->surface_capabilities);
  if (!hnd_assert(surface_capabilities_result == VK_SUCCESS, "Could not query surface capabilities"))
  {
#ifdef HND_DEBUG
    printf("         Code: %d", surface_capabilities_result);
#endif /* HND_DEBUG */
    
    return HND_NK;
  }

  uint32_t surface_format_count;
  vkGetPhysicalDeviceSurfaceFormatsKHR(_renderer->physical_device,
                                       _renderer->surface,
                                       &surface_format_count,
                                       NULL);
  if (!hnd_assert(surface_format_count > 0, "Not enough surface formats"))
    return HND_NK;
  VkSurfaceFormatKHR surface_formats[surface_format_count];
  vkGetPhysicalDeviceSurfaceFormatsKHR(_renderer->physical_device,
                                       _renderer->surface,
                                       &surface_format_count,
                                       surface_formats);

  uint32_t surface_present_mode_count;
  vkGetPhysicalDeviceSurfacePresentModesKHR(_renderer->physical_device,
                                            _renderer->surface,
                                            &surface_present_mode_count,
                                            NULL);
  if (!hnd_assert(surface_present_mode_count > 0, "Not enough surface present modes"))
    return HND_NK;
  VkPresentModeKHR surface_present_modes[surface_present_mode_count];
  vkGetPhysicalDeviceSurfacePresentModesKHR(_renderer->physical_device,
                                            _renderer->surface,
                                            &surface_present_mode_count,
                                            surface_present_modes);

  /** @note Picks the best swapchain surface format, based on each format's data. */
  VkSurfaceFormatKHR *best_surface_format = &surface_formats[0];
  int best_surface_format_score = 0;
  int current_surface_format_score = 0;
  for (uint32_t i = 1; i < surface_format_count; ++i)
  {
    current_surface_format_score = 0;
    
    if (surface_formats[i].format == VK_FORMAT_B8G8R8A8_SRGB)
      current_surface_format_score += 10;
    if (surface_formats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
      current_surface_format_score += 10;

    if (current_surface_format_score > best_surface_format_score)
    {
      best_surface_format_score = current_surface_format_score;
      best_surface_format = &surface_formats[i];
    }
  }
  _renderer->surface_format = *best_surface_format;

  /** @note Pick present mode */
  _renderer->surface_present_mode = VK_PRESENT_MODE_FIFO_KHR;
  for (uint32_t i = 0; i < surface_present_mode_count; ++i)
  {
    if (surface_present_modes[i] == VK_PRESENT_MODE_MAILBOX_KHR)
      _renderer->surface_present_mode = VK_PRESENT_MODE_MAILBOX_KHR;
  }

  /** @note Really weird implementation of clamp */
  uint32_t min_width = _renderer->surface_capabilities.minImageExtent.width;
  uint32_t max_width = _renderer->surface_capabilities.maxImageExtent.width;
  uint32_t min_height = _renderer->surface_capabilities.minImageExtent.height;
  uint32_t max_height = _renderer->surface_capabilities.maxImageExtent.height;
  
  _renderer->swapchain_extent.width =
    (_renderer->surface_capabilities.currentExtent.width < min_width)
      ? min_width
      : _renderer->surface_capabilities.currentExtent.width;
  
  _renderer->swapchain_extent.width =
    (_renderer->swapchain_extent.width > max_width)
      ? max_width
      : _renderer->swapchain_extent.width;
  
  _renderer->swapchain_extent.height =
    (_renderer->surface_capabilities.currentExtent.height < min_height)
      ? min_height
      : _renderer->surface_capabilities.currentExtent.height;
  
  _renderer->swapchain_extent.height =
    (_renderer->swapchain_extent.height > max_height)
      ? max_height
      : _renderer->swapchain_extent.height;
  
  /* Create swapchain */
  _renderer->swapchain_create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  _renderer->swapchain_create_info.surface = _renderer->surface;
  _renderer->swapchain_create_info.minImageCount = _renderer->surface_capabilities.minImageCount + 1;
  _renderer->swapchain_create_info.imageFormat = _renderer->surface_format.format;
  _renderer->swapchain_create_info.imageColorSpace = _renderer->surface_format.colorSpace;
  _renderer->swapchain_create_info.imageExtent = _renderer->swapchain_extent;
  _renderer->swapchain_create_info.imageArrayLayers = 1;
  _renderer->swapchain_create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
  _renderer->swapchain_create_info.preTransform = _renderer->surface_capabilities.currentTransform;
  _renderer->swapchain_create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
  _renderer->swapchain_create_info.presentMode = _renderer->surface_present_mode;
  _renderer->swapchain_create_info.clipped = VK_TRUE;
  _renderer->swapchain_create_info.oldSwapchain = VK_NULL_HANDLE; /* TODO(J0sueTM) */

  /** @note Differ queue families from surface and presentation  */
  uint32_t physical_device_queue_family_indices[2] =
  {
    _renderer->physical_device_queue_family_with_graphics_bit_index,
    _renderer->physical_device_queue_family_with_surface_presentation_support_index
  };
  
  if (_renderer->physical_device_queue_family_with_graphics_bit_index ==
      _renderer->physical_device_queue_family_with_surface_presentation_support_index)
    _renderer->swapchain_create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
  else
  {
    _renderer->swapchain_create_info.queueFamilyIndexCount = 2;
    _renderer->swapchain_create_info.pQueueFamilyIndices = physical_device_queue_family_indices;
    _renderer->swapchain_create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
  }

  VkResult swapchain_creation_result = vkCreateSwapchainKHR(_renderer->logical_device,
                                                            &_renderer->swapchain_create_info,
                                                            NULL,
                                                            &_renderer->swapchain);
  if (!hnd_assert(swapchain_creation_result == VK_SUCCESS, "Could not create swapchain"))
    return HND_NK;

  hnd_get_swapchain_images(_renderer);

  hnd_print_debug(HND_LOG, HND_CREATED("swapchain"), HND_SUCCESS);
  return HND_OK;
}

int
hnd_create_swapchain_image_views
(
  hnd_vulkan_renderer_t *_renderer
)
{
  if (!hnd_assert(_renderer != NULL, HND_SYNTAX))
    return HND_NK;

  _renderer->swapchain_image_view_create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
  _renderer->swapchain_image_view_create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
  _renderer->swapchain_image_view_create_info.format = _renderer->surface_format.format;
  _renderer->swapchain_image_view_create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
  _renderer->swapchain_image_view_create_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
  _renderer->swapchain_image_view_create_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
  _renderer->swapchain_image_view_create_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
  _renderer->swapchain_image_view_create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
  _renderer->swapchain_image_view_create_info.subresourceRange.baseMipLevel = 0;
  _renderer->swapchain_image_view_create_info.subresourceRange.levelCount = 1;
  _renderer->swapchain_image_view_create_info.subresourceRange.baseArrayLayer = 0;
  _renderer->swapchain_image_view_create_info.subresourceRange.layerCount = 1;

  /** @note Loops throughout retrieved images and create their views */
  VkResult swapchain_image_view_creation_info;
  for (uint32_t i = 0; i < _renderer->swapchain_image_count; ++i)
  {
    _renderer->swapchain_image_view_create_info.image = _renderer->swapchain_images[i];

    swapchain_image_view_creation_info = vkCreateImageView(_renderer->logical_device,
                                                           &_renderer->swapchain_image_view_create_info,
                                                           NULL,
                                                           &_renderer->swapchain_image_views[i]);
    if (!hnd_assert(swapchain_image_view_creation_info == VK_SUCCESS, "Could not create image view"))
      return HND_NK;
  }

  hnd_print_debug(HND_LOG, HND_CREATED("swapchain image views"), HND_SUCCESS);
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
  hnd_vulkan_renderer_t *_renderer
)
{
  /* IMPLEMENT ME(JsueTM) */
}