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

#include "../../core/event/event.h"
#include "../video.h"
#include "../renderer/renderer.h"

#define HND_WINDOW_CLASS_NAME "hound_window"
#define HND_WINDOW_DATA_PROPERTY "WINDOW_DATA_PROPERTY"
#define HND_WINDOW_EVENT_PROPERTY "WINDOW_EVENT_PROPERTY"

/**
 * @brief Win32 window data.
 */
typedef struct hnd_win32_window_t
{
  char *title;
  hnd_vector position;
  hnd_vector size;
  unsigned int decoration;
  unsigned int fullscreen;
  int running;

  RECT rect;
  RECT *global_rect;

  WNDCLASS class;
  HWND handle;
  DWORD style;
  DWORD extended_style;

  hnd_renderer_t renderer;
} hnd_win32_window_t;

/**
 * @brief Adds a property for the event so it can be called on the default callback.
 *
 * @param _window Specifies the window with the win32 handle.
 * @param _event  Specifies the event struct to bind.
 */
int
hnd_set_window_event
(
  hnd_win32_window_t *_window,
  hnd_event_t        *_event
);

/**
 * @brief Win32 default callback, called for event handling.
 *
 * @note https://docs.microsoft.com/en-us/previous-versions/windows/desktop/legacy/ms633573(v=vs.85)
 */
LRESULT CALLBACK
hnd_window_proc
(
  HWND   _handle,
  UINT   _message,
  WPARAM _wparam,
  LPARAM _lparam
);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __HND_WIN32_WINDOW_H__ */
