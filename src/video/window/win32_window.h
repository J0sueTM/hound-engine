/**
 * @file src/video/window/win32_window.h
 * @author Josue Teodoro Moreira <teodoro.josue@protonmail.ch>
 * @date August 25, 2021
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

#ifndef __HND_WIN32_WINDOW_H__
#define __HND_WIN32_WINDOW_H__

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#include "../video.h"
#include "../renderer/renderer.h"

#define HND_WINDOW_CLASS_NAME "hound_window"
#define HND_WINDOW_RENDERER_PROPERTY_NAME "window_renderer"

/**
 * @brief Windows window data.
 */
typedef struct hnd_win32_window_t
{
  char *title;
  unsigned int left;
  unsigned int top;
  unsigned int width;
  unsigned int height;
  unsigned int decoration;
  int running;

  WNDCLASS class;
  HWND handle;
  DWORD style;
  DWORD extended_style;

  hnd_renderer_t renderer;
} hnd_win32_window_t;

LRESULT CALLBACK
hnd_window_proc
(
  HWND   _window,
  UINT   _message,
  WPARAM _wparam,
  LPARAM _lparam
);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __HND_WIN32_WINDOW_H__ */
