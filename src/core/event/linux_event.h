/**
 * @file src/core/event/linux_event.h
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

#ifndef __HND_LINUX_EVENT_H__
#define __HND_LINUX_EVENT_H__

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */
  
#include "../../core/core.h"
#include "keyboard.h"
#include <X11/X.h>
#include <X11/Xlib.h>

/*
 * You can choose the size of the key history
 *
 * However, beware that Hound loops through all of them.
*/
#ifndef HND_MAX_KEYBOARD_KEY_HISTORY
#define HND_MAX_KEYBOARD_KEY_HISTORY 50
#endif /* HND_MAX_KEYBOARD_KEY_HISTORY */

/**
 * @brief Keeps track of current events and their states.
 */
typedef struct hnd_linux_event_t
{
  xcb_generic_event_t *xcb_event;

  /* keyboard */
  hnd_keyboard_key pressed_keys[HND_MAX_KEYBOARD_KEY_HISTORY];
  hnd_keyboard_key released_keys[HND_MAX_KEYBOARD_KEY_HISTORY];
  unsigned int pressed_key_history_count;
  unsigned int released_key_history_count;
} hnd_linux_event_t;

void
hnd_queue_key_event
(
  hnd_keyboard_key    *_key_history,
  xcb_generic_event_t *_xcb_event
);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __HND_LINUX_EVENT_H__ */
