/**
 * @file src/core/event/event.h
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

#ifndef __HND_EVENT_H__ 
#define __HND_EVENT_H__

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#include "../../core/core.h"
#include "../../video/video.h"

#ifdef HND_WIN32
#include "win32_event.h"
typedef hnd_win32_event_t hnd_event_t;
#else
#include "linux_event.h"
typedef hnd_linux_event_t hnd_event_t;
#endif /* HND_WIN32 */

#define HND_EVENT_NONE 0

/* @note Keyboard */
#define HND_EVENT_KEY_PRESS   1
#define HND_EVENT_KEY_RELEASE 2

/* @note Mouse */
#define HND_EVENT_MOUSE_MOVE              3
#define HND_EVENT_MOUSE_BUTTON_PRESS      4
#define HND_EVENT_MOUSE_BUTTON_RELEASE    5

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __HND_EVENT_H__ */
