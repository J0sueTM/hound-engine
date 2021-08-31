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

static LRESULT CALLBACK
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
  {
    hnd_renderer_t *renderer = (hnd_renderer_t *)GetProp(_window, HND_WINDOW_RENDERER_PROPERTY_NAME);
    
    hnd_init_renderer(renderer);
    ReleaseDC(_window, renderer->device_context);
    
  } break;
  case WM_SIZE:
    break;
  case WM_DESTROY:
  {
    hnd_renderer_t *renderer = (hnd_renderer_t *)GetProp(_window, HND_WINDOW_RENDERER_PROPERTY_NAME);
    
    hnd_end_renderer(renderer);
    PostQuitMessage(0);
    
  } break;
  case WM_CLOSE:
    break;
  case WM_ACTIVATEAPP:
    break;
  default:
    return DefWindowProc(_window, _message, _wparam, _lparam);
    
    break;
  }

  return 0;
}

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

  new_window->class.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
  new_window->class.lpfnWndProc = hnd_window_proc;
  new_window->class.hInstance = GetModuleHandle(NULL);
  new_window->class.lpszClassName = new_window->title;

  if (!hnd_assert(RegisterClass(&new_window->class), "Could not register window class"))
      return NULL;

  new_window->handle = CreateWindowEx(0,
                                      new_window->class.lpszClassName,
                                      new_window->title,
                                      WS_OVERLAPPEDWINDOW | WS_VISIBLE,
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
#endif /* HND_USE_OPENGL */

  /** @note Set renderer pointer as window property, so we can pull it on the window callback */
  SetProp(new_window->handle, HND_WINDOW_RENDERER_PROPERTY_NAME, (HANDLE)&new_window->renderer);
  
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
