/**
 * @file src/video/window.c
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

#include "window.h"

/**
 * @brief Connects to X server.
 *
 * @param _connection Specifies the hound connection where xcb connection resides.
 *
 * @return Function state. HND_OK or HND_NK.
 */
static int
hnd_connect_to_xcb
(
  hnd_window_t *_window
)
{
#ifdef HND_USE_OPENGL
  _window->renderer.display = XOpenDisplay((char *)NULL);
  if (!hnd_assert(_window->renderer.display != NULL, "Could not open display"))
    return HND_NK;

  _window->renderer.default_screen = DefaultScreen(_window->renderer.display);
  _window->connection = XGetXCBConnection(_window->renderer.display);
  if (!hnd_assert(_window->connection != NULL, "Could not connect to X display"))
    return HND_NK;

  XSetEventQueueOwner(_window->renderer.display, XCBOwnsEventQueue);
#else
  _window->connection = xcb_connect(NULL, &_window->renderer.default_screen);
  if (!hnd_assert(_window->connection != NULL, "Could not connect to X server"))
    return HND_NK;
#endif /* HND_USE_OPENGL */

  /* Get screen data */
  xcb_screen_iterator_t screen_iterator = xcb_setup_roots_iterator(xcb_get_setup(_window->connection));
  for (int i = _window->renderer.default_screen; i > 0; --i)
    xcb_screen_next(&screen_iterator);
  _window->screen_data = screen_iterator.data;

#ifdef HND_USE_OPENGL
  if (!hnd_set_fb_configs(&_window->renderer))
    return HND_NK;
#endif /* HND_USE_OPENGL */

  return HND_OK;
}


/**
 * @brief Sets window's title and adds close action listener.
 *
 * @param _window Specifies the window whose properties should change.
 */
static void
hnd_set_window_properties
(
  hnd_window_t *_window
)
{
  xcb_intern_atom_cookie_t utf8_string_cookie =
    hnd_intern_atom_cookie(_window->connection, "UTF8_STRING");
  xcb_intern_atom_cookie_t wm_name_cookie =
    hnd_intern_atom_cookie(_window->connection, "WM_NAME");
  xcb_intern_atom_cookie_t wm_delete_cookie =
    hnd_intern_atom_cookie(_window->connection, "WM_DELETE_WINDOW");
  xcb_intern_atom_cookie_t wm_protocols_cookie =
    hnd_intern_atom_cookie(_window->connection, "WM_PROTOCOLS");

  /* Set title */
  _window->utf8_string = hnd_intern_atom(_window->connection, utf8_string_cookie);
  _window->wm_name = hnd_intern_atom(_window->connection, wm_name_cookie);
  xcb_change_property(_window->connection,
                      XCB_PROP_MODE_REPLACE,
                      _window->id,
                      _window->wm_name,
                      _window->utf8_string,
                      8,
                      strlen(_window->title),
                      _window->title);

  /* Add listener for close event */
  _window->wm_protocols = hnd_intern_atom(_window->connection, wm_protocols_cookie);
  _window->wm_delete_window = hnd_intern_atom(_window->connection, wm_delete_cookie);
  xcb_change_property(_window->connection,
                      XCB_PROP_MODE_REPLACE,
                      _window->id,
                      _window->wm_protocols,
                      XCB_ATOM_ATOM,
                      32,
                      1,
                      &_window->wm_delete_window);
}

hnd_window_t *
hnd_create_window
(
  const char   *_title,
  unsigned int  _border,
  unsigned int  _left,
  unsigned int  _top,
  unsigned int  _width,
  unsigned int  _height
)
{
  hnd_window_t *new_window = malloc(sizeof(hnd_window_t));
  if (!hnd_assert(new_window != NULL, NULL))
    return NULL;

  if (!hnd_connect_to_xcb(new_window))
    return NULL;

  new_window->title = (char *)_title;
  new_window->border = _border;
  new_window->left = _left;
  new_window->top = _top;
  new_window->width = _width;
  new_window->height = _height;
  new_window->running = HND_NK;

  if (!hnd_init_renderer(&new_window->renderer))
  {
    hnd_destroy_window(new_window);

    return NULL;
  }

  /* Create window */
#ifdef HND_USE_OPENGL
  new_window->colormap_id = xcb_generate_id(new_window->connection);
  xcb_create_colormap(new_window->connection,
                      XCB_COLORMAP_ALLOC_NONE,
                      new_window->colormap_id,
                      new_window->screen_data->root,
                      new_window->renderer.visual_id);
#endif /* HND_USE_OPENGL */
  new_window->id = xcb_generate_id(new_window->connection);
  
  new_window->event_mask = XCB_EVENT_MASK_EXPOSURE       |
                           XCB_EVENT_MASK_BUTTON_PRESS   |
                           XCB_EVENT_MASK_BUTTON_RELEASE |
                           XCB_EVENT_MASK_POINTER_MOTION |
                           XCB_EVENT_MASK_BUTTON_MOTION  |
                           XCB_EVENT_MASK_ENTER_WINDOW   |
                           XCB_EVENT_MASK_LEAVE_WINDOW   |
                           XCB_EVENT_MASK_KEY_PRESS      |
                           XCB_EVENT_MASK_KEY_RELEASE;

  new_window->value_mask = XCB_CW_EVENT_MASK
#ifdef HND_USE_OPENGL
                         | XCB_CW_COLORMAP;
#else
                         ;
#endif /* HND_USE_OPENGL */

  new_window->value_list[0] = new_window->event_mask;
  new_window->value_list[2] = 0;
#ifdef HND_USE_OPENGL
  new_window->value_list[1] = new_window->colormap_id;
#else
  new_window->value_list[1] = 0;
#endif /* HND_USE_OPENGL */

  xcb_create_window(new_window->connection,
                    XCB_COPY_FROM_PARENT,
                    new_window->id,
                    new_window->screen_data->root,
                    new_window->left,
                    new_window->top,
                    new_window->width,
                    new_window->height,
                    new_window->border,
                    XCB_WINDOW_CLASS_INPUT_OUTPUT,
#ifdef HND_USE_OPENGL
                    new_window->renderer.visual_id,
#else
                    new_window->screen_data->root_visual,
#endif /* HND_USE_OPENGL */
                    new_window->value_mask,
                    new_window->value_list);

  hnd_set_window_properties(new_window);

  xcb_map_window(new_window->connection, new_window->id);
  xcb_flush(new_window->connection);

#ifdef HND_USE_OPENGL
  new_window->renderer.gl_window =
    glXCreateWindow(new_window->renderer.display,
                    new_window->renderer.current_fb_config,
                    new_window->id,
                    0);
  if (!hnd_assert(new_window->renderer.gl_window, "Could not create OpenGL window"))
  {
    hnd_destroy_window(new_window);
    
    return NULL;
  }

  if (!hnd_assert(glXMakeContextCurrent(new_window->renderer.display,
                                        new_window->renderer.gl_window,
                                        new_window->renderer.gl_window,
                                        new_window->renderer.gl_context),
                                        "Could not set OpenGL current rendering context"))
  {
    hnd_destroy_window(new_window);
    
    return NULL;
  }

  hnd_print_debug(HND_LOG, "Created OpenGL context", HND_SUCCESS);
#else
  hnd_create_surface(&new_window->renderer, new_window->connection, new_window->id);
  hnd_create_swapchain(&new_window->renderer);
  hnd_create_swapchain_image_views(&new_window->renderer);
#endif /* HND_USE_OPENGL */

  new_window->running = HND_OK;
  hnd_print_debug(HND_LOG, HND_CREATED("window"), HND_SUCCESS);
  
  return new_window;
}

void
hnd_destroy_window
(
  hnd_window_t *_window
)
{
  if (!hnd_assert(_window != NULL, HND_SYNTAX))
    return;

  hnd_end_renderer(&_window->renderer);

#ifdef HND_USE_OPENGL
  xcb_free_colormap(_window->connection, _window->colormap_id);
#else
  xcb_destroy_window(_window->connection, _window->id);
  xcb_disconnect(_window->connection);
#endif /* HND_USE_VULKAN */

  hnd_print_debug(HND_LOG, HND_ENDED("window"), HND_SUCCESS);
}

void
hnd_poll_events
(
  hnd_window_t     *_window,
  hnd_event_t      *_event
)
{
  if (!hnd_assert(_window != NULL, HND_SYNTAX))
    return;
  if (!hnd_assert(_event != NULL, HND_SYNTAX))
    return;

  _event->xcb_event = xcb_poll_for_event(_window->connection);
  if (!_event->xcb_event)
    return;
  
  _event->xcb_event->response_type &= ~0x80;
  uint8_t response_type = _event->xcb_event->response_type;
  switch (response_type)
  {
  case XCB_EXPOSE:
    xcb_flush(_window->connection);
    
    break;
  case XCB_KEY_PRESS:
    hnd_queue_key_event(_event->pressed_keys, _event->xcb_event);

    break;
  case XCB_CLIENT_MESSAGE:
  {
    xcb_client_message_event_t *temp_client_message_event = (xcb_client_message_event_t *)_event;

    if (temp_client_message_event->type == _window->wm_protocols &&
        temp_client_message_event->data.data32[0] == _window->wm_delete_window)
      _window->running = HND_NK;
  } break;
  default:
      break;
  }
}
