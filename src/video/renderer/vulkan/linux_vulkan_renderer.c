/**
 * @file src/video/renderer/vulkan/linux_vulkan_renderer.c
 * @author Josue Teodoro Moreira <teodoro.josue@protonmail.ch>
 * @date August 30, 2021
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

#include "linux_vulkan_renderer.h"

int
hnd_create_surface
(
  hnd_vulkan_renderer_t *_renderer,
  xcb_connection_t      *_connection,
  xcb_window_t           _window
)
{
  if (!hnd_assert(_renderer != NULL, HND_SYNTAX))
    return HND_NK;

  _renderer->xcb_surface_create_info.sType = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
  _renderer->xcb_surface_create_info.connection = _connection;
  _renderer->xcb_surface_create_info.window = _window;

  VkResult xcb_surface_creation_result = vkCreateXcbSurfaceKHR(_renderer->instance,
                                                               &_renderer->xcb_surface_create_info,
                                                               NULL,
                                                               &_renderer->surface);
  if (!hnd_assert(xcb_surface_creation_result == VK_SUCCESS, "Could not create xcb surface"))
    return HND_NK;


  /** @note Picks chosen physical device's queue family with support for surface presentation */
  uint32_t physical_device_queue_family_count;
  vkGetPhysicalDeviceQueueFamilyProperties(_renderer->physical_device, &physical_device_queue_family_count, NULL);

  /**
   * @note If there're not enough queue families, then we're supposing that the chosen rendering capable family
   * is the same as the presentation supported.
   */
  _renderer->physical_device_queue_family_with_surface_presentation_support_index =
    _renderer->physical_device_queue_family_with_graphics_bit_index;
  VkBool32 found_physical_device_queue_family_with_surface_presentation_support;
  if (hnd_assert(physical_device_queue_family_count > 0, "Not enough queue families"))
  {
    for (uint32_t i = 0; i < physical_device_queue_family_count; ++i)
    {
      vkGetPhysicalDeviceSurfaceSupportKHR(_renderer->physical_device,
                                           i,
                                           _renderer->surface,
                                           &found_physical_device_queue_family_with_surface_presentation_support);

      if (found_physical_device_queue_family_with_surface_presentation_support == VK_TRUE)
      {
        _renderer->physical_device_queue_family_with_surface_presentation_support_index = i;

        hnd_print_debug(HND_LOG, "Found queue family with surface presentation support", HND_SUCCESS);
#ifdef HND_DEBUG
        printf("       Code: %d\n", i);
#endif /* HND_DEBUG */
          
        break;
      }
    }
  }

  hnd_print_debug(HND_LOG, HND_CREATED("xcb surface"), HND_SUCCESS);
  return HND_OK;
}
