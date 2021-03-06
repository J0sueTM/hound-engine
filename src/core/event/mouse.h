/**
 * @file src/core/event/mouse.h
 * @author Josue Teodoro Moreira <teodoro.josue@protonmail.ch>
 * @date September 29, 2021
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

#ifndef __HND_LINUX_MOUSE_H__ 
#define __HND_LINUX_MOUSE_H__ 

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#define HND_MOUSE_BUTTON_UNKNOWN 0
#define HND_MOUSE_BUTTON_LEFT    1
#define HND_MOUSE_BUTTON_MIDDLE  2
#define HND_MOUSE_BUTTON_RIGHT   3

/* @note According to the definition in X11.h, the scroll up and scroll down events
 * are handled by X11 as button press and release events, one after another.
 *
 * @note https://stackoverflow.com/questions/15510472/scrollwheel-event-in-x11
 */
#define HND_MOUSE_BUTTON_MIDDLE_UP   4
#define HND_MOUSE_BUTTON_MIDDLE_DOWN 5

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* HND_LINUX_MOUSE_H */
