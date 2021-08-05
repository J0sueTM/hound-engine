/**
 * @file src/video/opengl_window.c
 * @author Josue Teodoro Moreira <teodoro.josue@protonmail.ch>
 * @date August 04, 2021
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
 *
 * @note: https://xcb.freedesktop.org/opengl/
 */

#include "window.h"

/**
 * @brief Frame buffer configs used on OpenGL context creation
 */
static int fb_configs[] =
{
  GLX_X_RENDERABLE,   True,
  GLX_DRAWABLE_TYPE,  GLX_WINDOW_BIT,
  GLX_RENDER_TYPE,    GLX_RGBA_BIT,
  GLX_X_VISUAL_TYPE,  GLX_TRUE_COLOR,
  GLX_RED_SIZE,       8,
  GLX_GREEN_SIZE,     8,
  GLX_BLUE_SIZE,      8,
  GLX_ALPHA_SIZE,     8,
  GLX_DEPTH_SIZE,     24,
  GLX_STENCIL_SIZE,   8,
  GLX_DOUBLEBUFFER,   True,
  GLX_SAMPLE_BUFFERS, 1,
  GLX_SAMPLES,        4,
  None
};

/**
 * @brief Gets and sets frame buffer config matching current screen's attributes.
 *
 * @param _connection Specifies the connection where fb data resides and
 *                    should be updated.
 *
 * @return Function state. HND_OK or HND_NK.
 */
static int
hnd_set_fb_configs
(
  hnd_connection_t *_connection
)
{
  _connection->fb_configs = glXChooseFBConfig(_connection->display,
                                              _connection->default_screen,
                                              fb_configs,
                                              &_connection->fb_config_count);
  if (!hnd_assert(_connection->fb_configs != 0, "Could not get frame buffer configs"))
    return HND_NK;

  /* Get the first of frame buffer config list */
  _connection->current_fb_config = *(_connection->fb_configs);
  glXGetFBConfigAttrib(_connection->display, _connection->current_fb_config, GLX_VISUAL_ID, &_connection->visual_id);
  
  return HND_OK;
}

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
  hnd_connection_t *_connection
)
{
  _connection->display = XOpenDisplay((char *)NULL);
  if (!hnd_assert(_connection->display != NULL, "Could not open display"))
    return HND_NK;

  _connection->default_screen = DefaultScreen(_connection->display);
  _connection->xcb_connection = XGetXCBConnection(_connection->display);
  if (!hnd_assert(_connection->xcb_connection != NULL, "Could not connect to X display"))
    return HND_NK;
  
  XSetEventQueueOwner(_connection->display, XCBOwnsEventQueue);

  /* Get screen data */
  xcb_screen_iterator_t screen_iterator = xcb_setup_roots_iterator(xcb_get_setup(_connection->xcb_connection));
  for (int i = _connection->default_screen; i > 0; --i)
    xcb_screen_next(&screen_iterator);
  _connection->screen_data = screen_iterator.data;

  if (!hnd_set_fb_configs(_connection))
    return HND_NK;

  return HND_OK;
}

hnd_window_t *
hnd_create_window
(
  const char   *_title,
  unsigned int  _left,
  unsigned int  _top,
  unsigned int  _width,
  unsigned int  _height
)
{
  hnd_window_t *new_window = malloc(sizeof(hnd_window_t));
  if (!hnd_assert(new_window != NULL, NULL))
    return NULL;

  if (!hnd_connect_to_xcb(&new_window->connection))
  {
    hnd_destroy_window(new_window);

    return NULL;
  }

  /* Create OpenGL rendering context */
  new_window->gl_context = glXCreateNewContext(new_window->connection.display,
                                               new_window->connection.current_fb_config,
                                               GLX_RGBA_TYPE,
                                               0,
                                               True);
  if (!hnd_assert(new_window->gl_context != NULL, "Could not create OpenGL rendering context"))
  {
    hnd_destroy_window(new_window);
    
    return NULL;
  }

  new_window->colormap_id = xcb_generate_id(new_window->connection.xcb_connection);
  xcb_create_colormap(new_window->connection.xcb_connection,
                      XCB_COLORMAP_ALLOC_NONE,
                      new_window->colormap_id,
                      new_window->connection.screen_data->root,
                      new_window->connection.visual_id);

  new_window->title = (char *)_title;
  new_window->width = _width;
  new_window->height = _height;

  /* Create window */
  new_window->id = xcb_generate_id(new_window->connection.xcb_connection);

  new_window->event_mask = XCB_EVENT_MASK_EXPOSURE       |
                           XCB_EVENT_MASK_BUTTON_PRESS   |
                           XCB_EVENT_MASK_BUTTON_RELEASE |
                           XCB_EVENT_MASK_POINTER_MOTION |
                           XCB_EVENT_MASK_BUTTON_MOTION  |
                           XCB_EVENT_MASK_ENTER_WINDOW   |
                           XCB_EVENT_MASK_LEAVE_WINDOW   |
                           XCB_EVENT_MASK_KEY_PRESS      |
                           XCB_EVENT_MASK_KEY_RELEASE;

  new_window->value_mask = XCB_CW_EVENT_MASK |
                           XCB_CW_COLORMAP;

  new_window->value_list[0] = new_window->event_mask;
  new_window->value_list[1] = new_window->colormap_id;
  new_window->value_list[2] = 0;

  xcb_create_window(new_window->connection.xcb_connection,
                    XCB_COPY_FROM_PARENT,
                    new_window->id,
                    new_window->connection.screen_data->root,
                    _left, _top,
                    _width, _height,
                    10,
                    XCB_WINDOW_CLASS_INPUT_OUTPUT,
                    new_window->connection.visual_id,
                    new_window->value_mask,
                    new_window->value_list);

  xcb_map_window(new_window->connection.xcb_connection, new_window->id);

  new_window->gl_window = glXCreateWindow(new_window->connection.display,
                                          new_window->connection.current_fb_config,
                                          new_window->id,
                                          0);
  if (!hnd_assert(new_window->gl_window, "Could not create OpenGL window"))
  {
    hnd_destroy_window(new_window);
    
    return NULL;
  }

  if (!hnd_assert(glXMakeContextCurrent(new_window->connection.display,
                                        new_window->gl_window,
                                        new_window->gl_window,
                                        new_window->gl_context),
                                        "Could not set OpenGL current rendering context"))
  {
    hnd_destroy_window(new_window);
    
    return NULL;
  }

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

  if (_window->gl_context)
    glXDestroyContext(_window->connection.display, _window->gl_context);
  if (_window->connection.display)
    XCloseDisplay(_window->connection.display);

  free(_window);

  hnd_print_debug(HND_LOG, HND_ENDED("window"), HND_SUCCESS);
}

void
hnd_clear_window
(
  float _red,
  float _green,
  float _blue,
  float _alpha
)
{
  glClearColor(_red, _green, _blue, _alpha);
  glClear(GL_COLOR_BUFFER_BIT);
}

void
hnd_swap_window_buffers
(
  hnd_window_t *_window
)
{
  glXSwapBuffers(_window->connection.display, _window->gl_window);
}
