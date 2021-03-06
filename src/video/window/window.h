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

#define HND_WINDOW_DECORATION_NONE     0x000
#define HND_WINDOW_DECORATION_MINIMIZE 0x001
#define HND_WINDOW_DECORATION_MAXIMIZE 0x010
#define HND_WINDOW_DECORATION_CLOSE    0x100
#define HND_WINDOW_DECORATION_ALL      0x111
 
/**
 * @brief Creates a window.
 *
 * @note If the window style doesn't display the upper bar, the
 *       title and the buttons won't be displayed as well.
 *
 * @param _title      Specifies the text to be displayed on the new window's upper bar.
 * @param _position   Specifies the position of the top left corner of the window.
 * @param _size       Specifies the width and the height of the window.
 * @param _decoration Specifies the window's decoration flags.
 *
 * @return The created window.
 */
hnd_window_t *
hnd_create_window
(
  const char   *_title,
  hnd_vector_t  _position,
  hnd_vector_t  _size,
  unsigned int  _decoration
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

/**
 * @brief Sets window position.
 *
 * @param _window   Specifies the window whose positios should be changed.
 * @param _position Specifies the new window position.
 *
 * @return Function state. HND_OK or HND_NK.
 */
int
hnd_set_window_position
(
  hnd_window_t *_window,
  hnd_vector_t  _position
);

/**
 * @brief Sets window decoration.
 *
 * @param _window     Specifies the window whose decoration should be changed.
 * @param _decoration Specifies the decoration flags that should change.
 *
 * @return Function state. HND_OK or HND_NK.
 */
int
hnd_set_window_decoration
(
  hnd_window_t *_window,
  unsigned int  _decoration
);

/**
 * @brief Sets a window's title.
 *
 * @param _window Specifies the window whose title should be setted.
 * @param _title  Specifies the new title.
 */
void
hnd_set_window_title
(
  hnd_window_t *_window,
  char         *_title
);
  
/**
 * @brief Sets given window's fullscreen state.
 *
 * @param _window     Specifies the window to make fullscreen, or not.
 * @param _fullscreen Specifies whether the window should be fullscreen or not.
 *
 * @return Function state. HND_OK or HND_NK.
 */
int
hnd_set_window_fullscreen
(
  hnd_window_t *_window,
  int           _fullscreen
);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __HND_WINDOW_H__ */
