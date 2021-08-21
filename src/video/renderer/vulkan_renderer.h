/**
 * @file src/video/renderer/opengl_renderer.h
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

#ifndef __HND_VULKAN_RENDERER_H__
#define __HND_VULKAN_RENDERER_H__

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#include "../video.h"

#ifndef HND_PROJECT_NAME
#define HND_PROJECT_NAME "Project made with Hound Engine"
#endif /* PROJECT_NAME */
  
#ifndef HND_PROJECT_VERSION_MAJOR
#define HND_PROJECT_VERSION_MAJOR 0
#endif /* HND_PROJECT_VERSION_MAJOR */

#ifndef HND_PROJECT_VERSION_MINOR
#define HND_PROJECT_VERSION_MINOR 0
#endif /* HND_PROJECT_VERSION_MINOR */

#ifndef HND_PROJECT_VERSION_PATCH
#define HND_PROJECT_VERSION_PATCH 1
#endif /* HND_PROJECT_VERSION_PATCH */

#ifdef HND_DEBUG
#define HND_INSTANCE_EXTENSION_COUNT 3
#else
#define HND_INSTANCE_EXTENSION_COUNT 2
#endif /* HND_DEBUG */
  
#define HND_VALIDATION_LAYER_COUNT 1

/**
 * @brief Vulkan renderer data.
 */
typedef struct hnd_vulkan_renderer_t
{
  int default_screen;

  /* Instance */
  VkApplicationInfo application_info;
  VkInstanceCreateInfo instance_create_info;
  VkInstance instance;
  char *instance_extensions[HND_INSTANCE_EXTENSION_COUNT];

  int are_validation_layers_supported;
#ifdef HND_DEBUG
  char *validation_layers[HND_VALIDATION_LAYER_COUNT];
  uint32_t supported_validation_layer_count;

  VkDebugUtilsMessengerCreateInfoEXT messenger_create_info;
  VkDebugUtilsMessengerEXT messenger;
#endif /* HND_DEBUG */

  /* Devices */
  VkPhysicalDevice physical_device;
  VkPhysicalDeviceFeatures physical_device_features;
  uint32_t physical_device_count;
  uint32_t physical_device_queue_family_with_graphics_bit_index;

  VkDevice logical_device;
  VkDeviceQueueCreateInfo logical_device_queue_create_info;
  VkDeviceCreateInfo logical_device_create_info;

} hnd_vulkan_renderer_t;
  
#ifdef HND_DEBUG
/**
 * @brief Checks if validation layers in hnd_vulkan_renderer_t->validation_layers
 *        are supported and can be enabled.
 *
 * @param _renderer Specifies the renderer containing the validation layers.
 */
void
hnd_check_validation_layers_support
(
  hnd_vulkan_renderer_t *_renderer
);

/**
 * @brief Creates vulkan validation layers debug messenger.
 *
 * @param _renderer Specifies the renderer containing data used to create the debug messenger.
 */
void
hnd_create_debug_messenger
(
  hnd_vulkan_renderer_t *_renderer
);
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
);

/**
 * @brief Creates vulkan instance.
 *
 * @param _renderer Specifies the renderer containing instance data.
 *
 * @return Function state. HND_OK or HND_NK.
 */
int
hnd_create_instance
(
  hnd_vulkan_renderer_t *_renderer
);

/**
 * @brief Gets vulkan devices.
 *
 * @note If HND_PICK_PHYSICAL_DEVICE is defined, Hound won't analyse all
 *       available physical devices, but pick HND_PICK_PHYSICAL_DEVICE, if not
 *       out of bound.
 *
 * @param _renderer Specifies the renderer containing physical device data.
 *
 * @return Function state. HND_OK or HND_NK.
 */
int
hnd_get_physical_devices
(
  hnd_vulkan_renderer_t *_renderer
);

int
hnd_create_logical_device
(
  hnd_vulkan_renderer_t *_renderer
);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __HND_VULKAN_RENDERER_H__ */
