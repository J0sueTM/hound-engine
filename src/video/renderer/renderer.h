/**
 * @file src/video/renderer/renderer.h
 * @author Josue Teodoro Moreira <teodoro.josue@protonmail.ch>
 * @date August 08, 2021
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

#ifndef __HND_RENDERER_H__
#define __HND_RENDERER_H__

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#include "../../core/core.h"
#include "../video.h"
 
#ifdef HND_WIN32
#include "win32_renderer.h"
typedef hnd_win32_renderer_t hnd_renderer_t;
#else
#include "linux_renderer.h"
typedef hnd_linux_renderer_t hnd_renderer_t;
#endif /* HND_WIN32 */

/**
 * @brief Initialises given renderer.
 *
 * @param _renderer Specifies the renderer to initialise.
 *
 * @return The function state. HND_OK or HND_NK.
 */
int
hnd_init_renderer
(
  hnd_renderer_t *_renderer
);

/**
 * @brief Ends given renderer.
 *
 * @param _renderer Specifies the renderer to end.
 */
void
hnd_end_renderer
(
  hnd_renderer_t *_renderer
);

/**
 * @brief Sets teh colour used to clear the window before redrawing.
 *
 * @param _red   Specifies the red channel.
 * @param _green Specifies the green channel.
 * @param _blue  Specifies the blue channel.
 * @param _alpha Specifies the alpha channel.
 */
void
hnd_set_renderer_clear_color
(
  float _red,
  float _green,
  float _blue,
  float _alpha
);

/**
 * @brief Clears the window.
 */
void
hnd_clear_render
(
  void
);

/**
 * @brief Swaps rendering buffers.
 *
 * @param _window Specifies window whose buffers should be swapped.
 */
void
hnd_swap_renderer_buffers
(
  hnd_renderer_t *_renderer
);

/**
 * @brief Sets new rendereing area.
 *
 * @param _width  Specifies the width of the new viewport.
 * @param _height Specifies the height of the new viewport.
 */
void
hnd_resize_renderer_viewport
(
  unsigned int _width,
  unsigned int _height
);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __HND_RENDERER_H__  */
