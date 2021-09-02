/**
 * @file src/core/debug.h
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

#ifndef __HND_DEBUG_H__
#define __HND_DEBUG_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#ifdef HND_WIN32
#include <windows.h>
#endif /* HND_WIN32 */

/* Default returns  */
#define HND_OK 1
#define HND_NK 0

/* Debug modes */
#define HND_LOG "[LOG]:"
#define HND_WARNING "[WARNING]:"
#define HND_ERROR "[ERROR]:"

/* Debug messages */
#define HND_CREATED(_object) "Created " _object
#define HND_ENDED(_object) "Ended " _object

/* Reasons */
#define HND_SUCCESS "Assertion succeded"
#define HND_FAILURE "Assertion failed"
#define HND_SYNTAX  "Incorrect syntax"

void
hnd_print_debug
(
  const char *_mode,
  const char *_message,
  const char *_reason
);

int
hnd_assert
(
  int         _assertion,
  const char *_reason
);

#if defined(HND_DEBUG) && defined(HND_USE_VULKAN)
VKAPI_ATTR VkBool32 VKAPI_CALL
vulkan_debug_callback
(
  VkDebugUtilsMessageSeverityFlagBitsEXT      _severity,
  VkDebugUtilsMessageTypeFlagsEXT             _type,
  const VkDebugUtilsMessengerCallbackDataEXT *_callback_data,
  void                                       *_user_data
);
#endif /* HND_DEBUG && HND_USE_VULKAN */

#endif /* __HND_DEBUG_H__ */
