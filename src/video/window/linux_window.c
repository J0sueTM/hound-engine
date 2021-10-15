/**
 * @file src/video/window/linux_window.c
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
 * @brief Connects to xcb/xli.
 *
 * @param _window Specifies the currently being created window where most of
 *                xcb/xlib data resides.
 *
 * @return The function state. HND_OK or HND_NK.
 */
static int
hnd_connect_to_xcb
(
  hnd_linux_window_t *_window
)
{
  _window->renderer.display = XOpenDisplay((char *)NULL);
  if (!hnd_assert(_window->renderer.display != NULL, "Could not open display"))
    return HND_NK;

  _window->renderer.default_screen = DefaultScreen(_window->renderer.display);
  _window->connection = XGetXCBConnection(_window->renderer.display);
  if (!hnd_assert(_window->connection != NULL, "Could not connect to X display"))
    return HND_NK;

  XSetEventQueueOwner(_window->renderer.display, XCBOwnsEventQueue);

  /* @note Get screen data */
  xcb_screen_iterator_t screen_iterator = xcb_setup_roots_iterator(xcb_get_setup(_window->connection));
  for (int i = _window->renderer.default_screen; i > 0; --i)
    xcb_screen_next(&screen_iterator);
  _window->screen_data = screen_iterator.data;

  return HND_OK;
}

/**
 * @brief Gets window's state atoms.
 *
 * @param _window Specifies the window to get atoms from.
 */
static void
hnd_get_window_atoms
(
  hnd_linux_window_t *_window
)
{
  /* @note Atom cookies */
  xcb_intern_atom_cookie_t wm_protocols = xcb_intern_atom(_window->connection,
                                                          1,
                                                          12,
                                                          "WM_PROTOCOLS");
  xcb_intern_atom_cookie_t wm_delete_window = xcb_intern_atom(_window->connection,
                                                              0,
                                                              16,
                                                              "WM_DELETE_WINDOW");

  /* @note Atom replies */
  _window->wm_protocols = xcb_intern_atom_reply(_window->connection, wm_protocols, NULL);
  _window->wm_delete_window = xcb_intern_atom_reply(_window->connection, wm_delete_window, NULL);

  /**
   * @note Sets a listener for a wm delete window event.
   *
   * Independent of window manager, pressing the close button (if the title bar is visible)
   * or sending a wm_close event to this created window will make xcb send us an event, so we can
   * cleanup and do necessary stuff before completely ending the application.
   */
  xcb_change_property(_window->connection,
                      XCB_PROP_MODE_REPLACE,
                      _window->id,
                      _window->wm_protocols->atom,
                      XCB_ATOM_ATOM,
                      32,
                      1,
                      &_window->wm_delete_window->atom);
}

hnd_linux_window_t *
hnd_create_window
(
  const char   *_title,
  unsigned int  _left,
  unsigned int  _top,
  unsigned int  _width,
  unsigned int  _height,
  unsigned int  _decoration
)
{
  hnd_linux_window_t *new_window = malloc(sizeof(hnd_linux_window_t));
  if (!hnd_assert(new_window != NULL, NULL))
    return NULL;

  new_window->title = (char *)_title;
  new_window->left = _left;
  new_window->top = _top;
  new_window->width = _width;
  new_window->height = _height;
  new_window->running = HND_OK;

  if (!hnd_connect_to_xcb(new_window))
    return NULL;

  if (!hnd_init_renderer(&new_window->renderer))
  {
    hnd_destroy_window(new_window);

    return NULL;
  }

  new_window->colormap_id = xcb_generate_id(new_window->connection);
  xcb_create_colormap(new_window->connection,
                      XCB_COLORMAP_ALLOC_NONE,
                      new_window->colormap_id,
                      new_window->screen_data->root,
                      new_window->renderer.visual_id);
  
  new_window->value_mask = XCB_CW_EVENT_MASK | XCB_CW_COLORMAP;

  new_window->event_mask = XCB_EVENT_MASK_EXPOSURE       |
                           XCB_EVENT_MASK_BUTTON_PRESS   |
                           XCB_EVENT_MASK_BUTTON_RELEASE |
                           XCB_EVENT_MASK_POINTER_MOTION |
                           XCB_EVENT_MASK_BUTTON_MOTION  |
                           XCB_EVENT_MASK_ENTER_WINDOW   |
                           XCB_EVENT_MASK_LEAVE_WINDOW   |
                           XCB_EVENT_MASK_KEY_PRESS      |
                           XCB_EVENT_MASK_KEY_RELEASE    |
                           XCB_EVENT_MASK_PROPERTY_CHANGE;

  new_window->value_list[0] = new_window->event_mask;
  new_window->value_list[1] = new_window->colormap_id;

  /* Create window */
  new_window->id = xcb_generate_id(new_window->connection);
  xcb_create_window(new_window->connection,
                    XCB_COPY_FROM_PARENT,
                    new_window->id,
                    new_window->screen_data->root,
                    new_window->left,
                    new_window->top,
                    new_window->width,
                    new_window->height,
                    0,
                    XCB_WINDOW_CLASS_INPUT_OUTPUT,
                    new_window->renderer.visual_id,
                    new_window->value_mask,
                    new_window->value_list);

  hnd_get_window_atoms(new_window);
  hnd_set_window_decoration(new_window, _decoration);

  xcb_map_window(new_window->connection, new_window->id);
  xcb_flush(new_window->connection);

  /* Finish opengl binding */
  new_window->renderer.gl_window = glXCreateWindow(new_window->renderer.display,
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
  
  hnd_print_debug(HND_LOG, HND_CREATED("OpenGL context"), HND_SUCCESS);

  hnd_print_debug(HND_LOG, HND_CREATED("window"), HND_SUCCESS);
  return new_window;
}

void
hnd_destroy_window
(
  hnd_linux_window_t *_window
)
{
  if (!hnd_assert(_window != NULL, HND_SYNTAX))
    return;

  hnd_end_renderer(&_window->renderer);
  xcb_free_colormap(_window->connection, _window->colormap_id);

  hnd_print_debug(HND_LOG, HND_ENDED("window"), HND_SUCCESS);
}

void
hnd_poll_events
(
  hnd_linux_window_t *_window,
  hnd_event_t        *_event
)
{
  if (!hnd_assert(_window != NULL, HND_SYNTAX))
    return;
  if (!hnd_assert(_event != NULL, HND_SYNTAX))
    return;

  _event->type = HND_EVENT_NONE;
  
  _event->keyboard.pressed_key = HND_KEY_UNKNOWN;
  _event->keyboard.released_key = HND_KEY_UNKNOWN;

  _event->xcb_event = xcb_poll_for_event(_window->connection);
  if (!_event->xcb_event)
  {
    _event->type = HND_EVENT_NONE;

    return;
  }
 
  switch (_event->xcb_event->response_type & 0x7f)
  {
  case XCB_CREATE_WINDOW:
    _event->type = HND_EVENT_NONE;
    
    _event->keyboard.pressed_key = HND_KEY_UNKNOWN;
    _event->keyboard.released_key = HND_KEY_UNKNOWN;
    
    break;
  case XCB_EXPOSE:
    xcb_flush(_window->connection);
    
    break;
  case XCB_KEY_PRESS:
  {
    _event->type = HND_EVENT_KEY_PRESS;

    xcb_key_press_event_t *temp_key_press_event = (xcb_key_press_event_t *)_event->xcb_event;
    _event->keyboard.pressed_key = temp_key_press_event->detail;

  } break;
  case XCB_KEY_RELEASE:
  {
    _event->type = HND_EVENT_KEY_RELEASE;

    /**
     * @bug I don't think it should be considered a bug, but, depending on the situation, this can
     * be intriguing:
     *
     * On some keyboards, the KEY_PRESS or KEY_RELEASE event doesn't happen forever. Instead, it presses
     * and releases it, and then begin spamming the pressed key. You probably saw this sometimes, when typing,
     * where you would click a key, and it would insert it, wait a couple milisseconds, and then begin spamming
     * that key.
     */
    xcb_key_release_event_t *temp_key_release_event = (xcb_key_release_event_t *)_event->xcb_event;
    _event->keyboard.released_key = temp_key_release_event->detail;
  
  } break;
  case XCB_BUTTON_PRESS:
  {
    _event->type = HND_EVENT_MOUSE_BUTTON_PRESS;

    xcb_button_press_event_t *temp_button_press_event = (xcb_button_press_event_t *)_event->xcb_event;
    _event->mouse.pressed_button = temp_button_press_event->detail;

  } break;
  case XCB_BUTTON_RELEASE:
  {
    _event->type = HND_EVENT_MOUSE_BUTTON_RELEASE;

    xcb_button_release_event_t *temp_button_release_event = (xcb_button_release_event_t *)_event->xcb_event;
    _event->mouse.released_button = temp_button_release_event->detail;

  } break;
  case XCB_CLIENT_MESSAGE:
  {
    xcb_client_message_event_t *temp_client_message_event = (xcb_client_message_event_t *)_event->xcb_event;

    if (temp_client_message_event->data.data32[0] == _window->wm_delete_window->atom)
      _window->running = HND_NK;
    
  } break;
  default:
      break;
  }
}

int
hnd_set_window_decoration
(
  hnd_linux_window_t *_window,
  unsigned int        _decoration
)
{
  if (!hnd_assert(_window != NULL, HND_SYNTAX))
    return HND_NK;

  _window->decoration = _decoration;

  /* @todo Handle decoration flags */
    
  return HND_OK;
}

void
hnd_set_window_title
(
  hnd_window_t *_window,
  char         *_title
)
{
  if (!hnd_assert(_window != NULL, HND_SYNTAX))
    return;

  _window->title = _title;
  
  /* @note Sets the window's title */
  xcb_change_property(_window->connection,
                      XCB_PROP_MODE_REPLACE,
                      _window->id,
                      XCB_ATOM_WM_NAME,
                      XCB_ATOM_STRING,
                      8,
                      strlen(_window->title),
                      _window->title);

  xcb_flush(_window->connection);
}

int
hnd_set_window_fullscreen
(
  hnd_linux_window_t *_window,
  int                 _fullscreen
)
{
  if (!hnd_assert(_window != NULL, HND_SYNTAX))
    return HND_NK;

  _window->fullscreen = _fullscreen;

  /* @todo implement me */

  return HND_OK;
}
