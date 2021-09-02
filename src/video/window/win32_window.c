/**
 * @file src/video/window/win32_window.c
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

#include "window.h"

hnd_win32_window_t *
hnd_create_window
(
  const char   *_title,
  unsigned int  _left,
  unsigned int  _top,
  unsigned int  _width,
  unsigned int  _height
)
{
  hnd_win32_window_t *new_window = malloc(sizeof(hnd_win32_window_t));
  if (!hnd_assert(new_window != NULL, NULL))
    return NULL;

  new_window->title = (char *)_title;
  new_window->left = _left;
  new_window->top = _top;
  new_window->width = _width;
  new_window->height = _height;
  new_window->running = HND_OK;
  
 /* @note Make sure not assigned values are assigned zero */
  memset((void *)&new_window->class, 0x00, sizeof(WNDCLASS));
  new_window->class.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
  new_window->class.lpfnWndProc = (WNDPROC)hnd_window_proc;
  new_window->class.hInstance = GetModuleHandle(NULL);
  new_window->class.lpszClassName = HND_WINDOW_CLASS_NAME;
  if (!hnd_assert(RegisterClass(&new_window->class), "Could not register window class"))
    return NULL;

  new_window->style = WS_OVERLAPPEDWINDOW;
  new_window->extended_style = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
  AdjustWindowRectEx(&((RECT){
                               (long)new_window->left,
                               (long)new_window->top,
                               (long)new_window->width,
                               (long)new_window->height
                             }),
                     new_window->style,
                     FALSE,
                     new_window->extended_style);

  new_window->handle = CreateWindowEx(new_window->extended_style,
                                      new_window->class.lpszClassName,
                                      new_window->title,
                                      WS_CLIPSIBLINGS | WS_CLIPCHILDREN | new_window->style | WS_VISIBLE,
                                      new_window->left,
                                      new_window->top,
                                      new_window->width,
                                      new_window->height,
                                      NULL,
                                      NULL,
                                      new_window->class.hInstance,
                                      NULL);
  if (!hnd_assert(new_window->handle != NULL, "Could not create window"))
    return NULL;

#ifdef HND_USE_OPENGL
  new_window->renderer.device_context = GetDC(new_window->handle);
  if (!hnd_assert(new_window->renderer.device_context != NULL, "Could not get window's device context"))
    return NULL;
#endif /* HND_USE_OPENGL */

  if (!hnd_init_renderer(&new_window->renderer))
    return NULL;
  
  hnd_print_debug(HND_LOG, HND_CREATED("window"), HND_SUCCESS);
  return new_window;
}

void
hnd_destroy_window
(
  hnd_win32_window_t *_window
)
{
  if (!hnd_assert(_window != NULL, HND_SYNTAX))
    return;

  hnd_end_renderer(&_window->renderer);
#ifdef HND_USE_OPENGL
  ReleaseDC(_window->handle, _window->renderer.device_context);
#endif /* HND_USE_OPENGL */
    
  DestroyWindow(_window->handle);
  UnregisterClass(HND_WINDOW_CLASS_NAME, GetModuleHandle(NULL));

  hnd_print_debug(HND_LOG, HND_ENDED("window"), HND_SUCCESS);
}

void
hnd_poll_events
(
  hnd_win32_window_t *_window,
  hnd_event_t        *_event
)
{
  if (!hnd_assert(_window != NULL, HND_SYNTAX))
    return;
  if (!hnd_assert(_event != NULL, HND_SYNTAX))
    return;

  _event->message_result = GetMessage(&_event->message, NULL, 0, 0);
  if (_event->message_result < 0)
  {
    _window->running = HND_NK;
    
    return;
  }

  TranslateMessage(&_event->message);
  DispatchMessage(&_event->message);
}

LRESULT CALLBACK
hnd_window_proc
(
  HWND   _window,
  UINT   _message,
  WPARAM _wparam,
  LPARAM _lparam
)
{
  switch (_message)
  {
  case WM_CREATE:
    break;
  case WM_SIZE:
    hnd_resize_renderer_viewport(LOWORD(_lparam), HIWORD(_wparam));
    
    break;
  case WM_DESTROY:
    PostQuitMessage(0);
    
    break;
  case WM_CLOSE:
    PostQuitMessage(0);
    
    break;
  case WM_ACTIVATEAPP:
    break;
  default:
    return DefWindowProc(_window, _message, _wparam, _lparam);
    
    break;
  }

  return 0;
}
