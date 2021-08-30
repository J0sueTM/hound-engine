/**
 * @file src/video/video.c
 * @author Josue Teodoro Moreira <teodoro.josue@protonmail.ch>
 * @date July 08, 2021
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

#include "video.h"

#ifdef HND_LINUX
xcb_intern_atom_cookie_t
hnd_intern_atom_cookie
(
  xcb_connection_t *_connection,
  const char       *_string
)
{
  return xcb_intern_atom(_connection, HND_NK, strlen(_string), _string);
}

xcb_atom_t
hnd_intern_atom
(
  xcb_connection_t         *_connection,
  xcb_intern_atom_cookie_t  _cookie
)
{
  xcb_atom_t new_atom = XCB_ATOM_NONE;
  xcb_intern_atom_reply_t *reply = xcb_intern_atom_reply(_connection, _cookie, NULL);
  if (!hnd_assert(reply != NULL, "Could not create atom"))
    return HND_NK;

  new_atom = reply->atom;
  free(reply);

  return new_atom;
}
#endif /* HND_LINUX */
