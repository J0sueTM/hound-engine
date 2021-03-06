/**
 * @file src/video/video.h
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

#ifndef __HND_VIDEO_H__
#define __HND_VIDEO_H__

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#include "../core/core.h"

#ifdef HND_LINUX
#include <X11/Xlib.h>
#include <X11/Xlib-xcb.h>
#include <xcb/xcb.h>
#endif /* HND_WIN32 */

#include <GL/gl.h>
#ifdef HND_LINUX
#include <GL/glx.h>
#endif /* HND_LINUX */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __HND_VIDEO_H__ */
