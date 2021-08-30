/**
 * @file src/core/event/linux_event.c
 * @author Josue Teodoro Moreira <teodoro.josue@protonmail.ch>
 * @date August 04, 2021
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

#include "event.h"

void
hnd_queue_key_event
(
  hnd_keyboard_key    *_key_history,
  xcb_generic_event_t *_xcb_event
)
{
  xcb_key_press_event_t *temp_key_event = (xcb_key_press_event_t *)_xcb_event;
  
  /* Iterates through the key history, moving each key backwards */
  for (int i = 1; i < HND_MAX_KEYBOARD_KEY_HISTORY; ++i)
    _key_history[i] = _key_history[i - 1];

  /* Replaces the first with the new event */
  _key_history[0] = temp_key_event->detail;
}
