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
  hnd_vector    _position,
  hnd_vector    _size,
  unsigned int  _decoration
)
{
  hnd_win32_window_t *new_window = malloc(sizeof(hnd_win32_window_t));
  if (!hnd_assert(new_window != NULL, NULL))
    return NULL;

  new_window->title = (char *)_title;
  hnd_copy_vector(_position, new_window->position);
  hnd_copy_vector(_size, new_window->size);
  new_window->running = HND_OK;
  
 /* @note Make sure not assigned values are assigned zero */
  memset((void *)&new_window->class, 0x00, sizeof(WNDCLASS));
  new_window->class.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
  new_window->class.lpfnWndProc = (WNDPROC)hnd_window_proc;
  new_window->class.hInstance = GetModuleHandle(NULL);
  new_window->class.lpszClassName = HND_WINDOW_CLASS_NAME;
  if (!hnd_assert(RegisterClass(&new_window->class), "Could not register window class"))
    return NULL;

  new_window->rect =
    (RECT)
    {
      (long)new_window->position[0],
      (long)new_window->position[1],
      (long)(new_window->position[0] + new_window->size[0]),
      (long)(new_window->position[1] + new_window->size[1])
    };

  new_window->style = WS_OVERLAPPEDWINDOW;
  new_window->extended_style = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
  AdjustWindowRectEx(&new_window->rect,
                     new_window->style,
                     FALSE,
                     new_window->extended_style);

  new_window->handle = CreateWindowEx(new_window->extended_style,
                                      new_window->class.lpszClassName,
                                      new_window->title,
                                      WS_CLIPSIBLINGS | WS_CLIPCHILDREN | new_window->style | WS_VISIBLE,
                                      new_window->position[0],
                                      new_window->position[1],
                                      new_window->size[0],
                                      new_window->size[1],
                                      GetDesktopWindow(),
                                      NULL,
                                      new_window->class.hInstance,
                                      NULL);
  if (!hnd_assert(new_window->handle != NULL, "Could not create window"))
    return NULL;

  /* @note Sets the window struct as a property assigned to it's window->handle so we can retrieve it
   * later on functions like hnd_window_proc.
   *
   * @note There's an issue with SetProp and GetProp, where GetProp was returning an error when it can't validate
   * HWND. Turned out I was running GetProp too early. In any case, GetProp should run after the window was created,
   * so it doesn't break on the first call to hnd_window_proc, since the first one would be WM_CREATE, when the
   * following code haven't even run yet (assigning the new property).
   */
  if (!hnd_assert(SetProp(new_window->handle, HND_WINDOW_DATA_PROPERTY, new_window),
                  "Could not set window data as property"))
    return NULL;

  /* @note Ends renderer binding: Device context */
  new_window->renderer.device_context = GetDC(new_window->handle);
  if (!hnd_assert(new_window->renderer.device_context != NULL, "Could not get window's device context"))
    return NULL;

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
  ReleaseDC(_window->handle, _window->renderer.device_context);

  RemoveProp(_window->handle, HND_WINDOW_DATA_PROPERTY);
  UnregisterClass(HND_WINDOW_CLASS_NAME, GetModuleHandle(NULL));

  hnd_print_debug(HND_LOG, HND_ENDED("window"), HND_SUCCESS);
}

int
hnd_set_window_event
(
  hnd_win32_window_t *_window,
  hnd_event_t        *_event
)
{
  if (!hnd_assert(_window != NULL, HND_SYNTAX))
    return HND_NK;
  if (!hnd_assert(_event != NULL, HND_SYNTAX))
    return HND_NK;

  if (!hnd_assert(SetProp(_window->handle, HND_WINDOW_EVENT_PROPERTY, _event), "Could not set event as property"))
    return HND_NK;

  return HND_OK;
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
    DestroyWindow(_window->handle);
    
    return;
  }

  TranslateMessage(&_event->message);
  DispatchMessage(&_event->message);
}

LRESULT CALLBACK
hnd_window_proc
(
  HWND   _handle,
  UINT   _message,
  WPARAM _wparam,
  LPARAM _lparam
)
{
  switch (_message)
  {
  case WM_SIZE:
    hnd_resize_renderer_viewport(LOWORD(_lparam), HIWORD(_wparam));

    break;
  case WM_DESTROY:
  {
    hnd_win32_window_t *temp_window = (hnd_win32_window_t *)GetProp(_handle, HND_WINDOW_DATA_PROPERTY);
    temp_window->running = HND_NK;

  } break;
  case WM_CLOSE:
    DestroyWindow(_handle);
    
    break;
  case WM_KEYDOWN:
  {
    hnd_event_t *temp_event = (hnd_event_t *)GetProp(_handle, HND_WINDOW_EVENT_PROPERTY);
    temp_event->type = HND_EVENT_KEY_PRESS;
    temp_event->keyboard.pressed_key = _wparam;

  } break;
  case WM_KEYUP:
  {
    hnd_event_t *temp_event = (hnd_event_t *)GetProp(_handle, HND_WINDOW_EVENT_PROPERTY);
    temp_event->type = HND_EVENT_KEY_RELEASE;
    temp_event->keyboard.released_key = _wparam;

  } break;
  case WM_LBUTTONDOWN:
  {
    hnd_event_t *temp_event = (hnd_event_t *)GetProp(_handle, HND_WINDOW_EVENT_PROPERTY);
    temp_event->type = HND_EVENT_MOUSE_BUTTON_PRESS;
    temp_event->mouse.pressed_button = HND_MOUSE_BUTTON_LEFT;
    temp_event->mouse.pressed_position[0] = (float)LOWORD(_lparam);
    temp_event->mouse.pressed_position[1] = (float)HIWORD(_lparam);

  } break;
  case WM_LBUTTONUP:
  {
    hnd_event_t *temp_event = (hnd_event_t *)GetProp(_handle, HND_WINDOW_EVENT_PROPERTY);
    temp_event->type = HND_EVENT_MOUSE_BUTTON_RELEASE;
    temp_event->mouse.released_button = HND_MOUSE_BUTTON_LEFT;
    temp_event->mouse.released_position[0] = (float)LOWORD(_lparam);
    temp_event->mouse.released_position[1] = (float)HIWORD(_lparam);

  } break;
  case WM_RBUTTONDOWN:
  {
    hnd_event_t *temp_event = (hnd_event_t *)GetProp(_handle, HND_WINDOW_EVENT_PROPERTY);
    temp_event->type = HND_EVENT_MOUSE_BUTTON_PRESS;
    temp_event->mouse.pressed_button = HND_MOUSE_BUTTON_RIGHT;
    temp_event->mouse.pressed_position[0] = (float)LOWORD(_lparam);
    temp_event->mouse.pressed_position[1] = (float)HIWORD(_lparam);

  } break;
  case WM_RBUTTONUP:
  {
    hnd_event_t *temp_event = (hnd_event_t *)GetProp(_handle, HND_WINDOW_EVENT_PROPERTY);
    temp_event->type = HND_EVENT_MOUSE_BUTTON_RELEASE;
    temp_event->mouse.released_button = HND_MOUSE_BUTTON_RIGHT;
    temp_event->mouse.released_position[0] = (float)LOWORD(_lparam);
    temp_event->mouse.released_position[1] = (float)HIWORD(_lparam);

  } break;
  case WM_MBUTTONDOWN:
  {
    hnd_event_t *temp_event = (hnd_event_t *)GetProp(_handle, HND_WINDOW_EVENT_PROPERTY);
    temp_event->type = HND_EVENT_MOUSE_BUTTON_PRESS;
    temp_event->mouse.pressed_button = HND_MOUSE_BUTTON_MIDDLE;
    temp_event->mouse.pressed_position[0] = (float)LOWORD(_lparam);
    temp_event->mouse.pressed_position[1] = (float)HIWORD(_lparam);

  } break;
  case WM_MBUTTONUP:
  {
    hnd_event_t *temp_event = (hnd_event_t *)GetProp(_handle, HND_WINDOW_EVENT_PROPERTY);
    temp_event->type = HND_EVENT_MOUSE_BUTTON_RELEASE;
    temp_event->mouse.released_button = HND_MOUSE_BUTTON_MIDDLE;
    temp_event->mouse.released_position[0] = (float)LOWORD(_lparam);
    temp_event->mouse.released_position[1] = (float)HIWORD(_lparam);

  } break;
  case WM_MOUSEMOVE:
  {
    hnd_event_t *temp_event = (hnd_event_t *)GetProp(_handle, HND_WINDOW_EVENT_PROPERTY);
    temp_event->type = HND_EVENT_MOUSE_MOVE;
    temp_event->mouse.position[0] = (float)LOWORD(_lparam); 
    temp_event->mouse.position[1] = (float)HIWORD(_lparam);

  } break;
  case WM_MOUSEWHEEL:
  {
    /* @note https://stackoverflow.com/questions/9245303/rawinput-how-to-get-mouse-wheel-data */
    hnd_event_t *temp_event = (hnd_event_t *)GetProp(_handle, HND_WINDOW_EVENT_PROPERTY);
    temp_event->type = HND_EVENT_MOUSE_BUTTON_PRESS;
    temp_event->mouse.pressed_button =
      ((short)(unsigned short)HIWORD(_wparam) < 0)
        ? HND_MOUSE_BUTTON_MIDDLE_DOWN
        : HND_MOUSE_BUTTON_MIDDLE_UP;
    temp_event->mouse.pressed_position[0] = (float)LOWORD(_lparam); 
    temp_event->mouse.pressed_position[1] = (float)HIWORD(_lparam);

  } break;
  default:
    return DefWindowProc(_handle, _message, _wparam, _lparam);
    
    break;
  }

  return 0;
}

int
hnd_set_window_fullscreen
(
  hnd_win32_window_t *_window,
  int                 _fullscreen
)
{
  if (!hnd_assert(_window != NULL, HND_SYNTAX))
    return HND_NK;

  _window->fullscreen = _fullscreen;

  /* @todo IMPLEMENT ME */

  return HND_OK;
}

void
hnd_set_window_title
(
  hnd_win32_window_t *_window,
  char               *_title
)
{
  if (!hnd_assert(_window != NULL, HND_SYNTAX))
    return;

  _window->title = _title;

  /* @todo IMPLEMENT ME */
}

int
hnd_set_window_decoration
(
  hnd_win32_window_t *_window,
  unsigned int        _decoration
)
{
  if (!hnd_assert(_window != NULL, HND_SYNTAX))
    return HND_NK;

  _window->decoration = _decoration;

  /* @todo IMPLEMENT ME */

  return HND_OK;
}
