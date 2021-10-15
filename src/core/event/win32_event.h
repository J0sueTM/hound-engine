/**
 * @file src/core/event/win32_event.h
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

#ifndef __HND_WIN32_EVENT_H__
#define __HND_WIN32_EVENT_H__

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */
  
#include "../../core/core.h"
#include "win32_keyboard.h"
#include "mouse.h"
/**
 * @brief Keeps track of current events and their states.
 */
typedef struct hnd_win32_event_t
{
  unsigned int type;
  MSG message;
  BOOL message_result;

  struct
  {
    unsigned int pressed_key;
    unsigned int released_key;
  } keyboard;

  struct
  {
    unsigned int pressed_button;
    unsigned int released_button;
  } mouse;
} hnd_win32_event_t;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __HND_WIN32_EVENT_H__ */
