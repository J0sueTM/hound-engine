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

#ifdef HND_USE_OPENGL
#include <GL/gl.h>
#ifdef HND_LINUX
#include <GL/glx.h>
#endif /* HND_LINUX */
#elif HND_USE_VULKAN
#include <vulkan/vulkan.h>
#ifdef HND_LINUX
#include <vulkan/vulkan_xcb.h>
#endif /* HND_LINUX */
#endif /* HND_USE_OPENGL */

#ifdef HND_LINUX
/**
 * @brief Creates an atom cookie.
 *
 * @param _connection Specifies the Xorg/Xcb server connection.
 * @param _string     Specifies the new atom's id name.
 *
 * @return The created atom cookie.
 */
xcb_intern_atom_cookie_t
hnd_intern_atom_cookie
(
  xcb_connection_t *_connection,
  const char       *_string
);

/**
 * @brief Creates an atom.
 *
 * @param _connection Specifies the Xorg/Xcb server connection.
 * @param _cookie     Specifies the atom cookie the new atom should be based off.
 *
 * @return The created atom.
 */
xcb_atom_t
hnd_intern_atom
(
  xcb_connection_t         *_connection,
  xcb_intern_atom_cookie_t  _cookie
);
#endif /* HND_LINUX */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __HND_VIDEO_H__ */
