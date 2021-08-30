/**
 * @file src/video/window/win32_window.c
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

#include "window.h"

hnd_win32_window_t *
hnd_create_window
(
  const char   *_title,
  unsigned int  _border,
  unsigned int  _left,
  unsigned int  _top,
  unsigned int  _width,
  unsigned int  _height
)
{
  /* IMPLEMENT ME(J0sueTM) */
  return NULL;
}

void
hnd_destroy_window
(
  hnd_win32_window_t *_window
)
{
  /* IMPLEMENT ME(J0sueTM) */
}

void
hnd_poll_events
(
  hnd_win32_window_t *_window,
  hnd_event_t        *_event
)
{
  /* IMPLEMENT ME(J0sueTM) */
}
