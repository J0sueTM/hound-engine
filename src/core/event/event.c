/**
 * @file src/core/event/event.c
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

static void
hnd_queue_key_event
(
  hnd_keyboard_key      *_key_history,
  xcb_generic_event_t   *_xcb_event
)
{
  xcb_key_press_event_t *temp_key_event = (xcb_key_press_event_t *)_xcb_event;
  
  /* Iterates through the key history, moving each key backwards */
  for (int i = 1; i < HND_MAX_KEYBOARD_KEY_HISTORY; ++i)
    _key_history[i] = _key_history[i - 1];

  /* Replaces the first with the new event */
  _key_history[0] = temp_key_event->detail;
}

void
hnd_poll_window_events
(
  xcb_connection_t *_connection,
  hnd_event_t      *_event
)
{
  if (!hnd_assert(_connection != NULL, HND_SYNTAX))
    return;
  if (!hnd_assert(_event != NULL, HND_SYNTAX))
    return;

  _event->xcb_event = xcb_poll_for_event(_connection);
  if (!_event->xcb_event)
    return;
  _event->xcb_event->response_type &= ~0x80;

  if (_event->xcb_event->response_type == XCB_KEY_PRESS)
    hnd_queue_key_event(_event->pressed_keys, _event->xcb_event);
}
