/**
 * @file src/video/window.h
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

#ifndef __HND_WINDOW_H__
#define __HND_WINDOW_H__

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#include "../../core/core.h"
#include "../../core/event/event.h"
#include "../video.h"
#include "../renderer/renderer.h"

#ifdef HND_WIN32
#include "win32_window.h"
typedef hnd_win32_window_t hnd_window_t;
#else
#include "linux_window.h"
typedef hnd_linux_window_t hnd_window_t;
#endif /* HND_WIN32 */
  
/**
 * @brief Creates a window.
 *
 * @note If the window style doesn't display the upper bar, the
 *       title won't be displayed as well.
 *
 * @param _title  Specifies the text to be displayed on the new window's upper bar.
 * @param _left   Specifies the distance from the left side of the screen to the upper
                  left corner of the window.
 * @param _top    Specifies the distance from the top of the screen to the upper left
                  corner of the window.
 * @param _width  Specifies the window's width.
 * @param _height Specifies the window's height.
 *
 * @return The created window.
 */
hnd_window_t *
hnd_create_window
(
  const char   *_title,
  unsigned int  _left,
  unsigned int  _top,
  unsigned int  _width,
  unsigned int  _height
);

/**
 * @brief Ends a window.
 *
 * @param _window Specifies the window to be destroyed.
 */
void
hnd_destroy_window
(
  hnd_window_t *_window
);

/**
 * @brief Gets the current event when active on window.
 *
 * @param _window Specifies the window where event should happen.
 * @param _event  Specifies the event struct where to allocate the poll.
 */
void
hnd_poll_events
(
  hnd_window_t *_window,
  hnd_event_t  *_event
);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __HND_WINDOW_H__ */