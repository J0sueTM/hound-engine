/**
 * @file src/core/debug.c
 * @author Josue Teodoro Moreira <teodoro.josue@protonmail.ch>
 * @date July 27, 2021
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

#include "debug.h"

void
hnd_print_debug
(
  const char *_mode,
  const char *_message,
  const char *_reason
)
{
  if (_message)
    printf("%s %s - %s.\n", _mode, _message, _reason);
  else
    printf("%s %d:%s - %s.\n", _mode, errno, strerror(errno), _reason);
}

int
hnd_assert
(
  int         _assertion,
  const char *_reason
)
{
  if (!(_assertion))
    hnd_print_debug(HND_ERROR, _reason, HND_FAILURE);
    
  return _assertion;
}

#if defined(HND_DEBUG) && defined(HND_USE_VULKAN)
VKAPI_ATTR VkBool32 VKAPI_CALL
vulkan_debug_callback
(
  VkDebugUtilsMessageSeverityFlagBitsEXT      _severity,
  VkDebugUtilsMessageTypeFlagsEXT             _type,
  const VkDebugUtilsMessengerCallbackDataEXT *_callback_data,
  void                                       *_user_data
)
{
  /* We don't really care about logs */
  if (_severity < VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
    return VK_FALSE;

  char *mode;
  
  if (_severity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
    mode = HND_WARNING;
  else
    mode = HND_ERROR;

  hnd_print_debug(mode, _callback_data->pMessage, HND_FAILURE);

  return VK_FALSE;
}
#endif /* HND_DEBUG && HND_USE_VULKAN */
