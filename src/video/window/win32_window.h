/**
 * @file src/video/window/win32_window.h
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

#ifndef __HND_WIN32_WINDOW_H__
#define __HND_WIN32_WINDOW_H__

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/**
 * @brief Linux window data.
 */
typedef struct hnd_win32_window_t
{
  char *title;
  unsigned int border;
  unsigned int left;
  unsigned int top;
  unsigned int width;
  unsigned int height;
  int running;
} hnd_win32_window_t;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __HND_WIN32_WINDOW_H__ */
