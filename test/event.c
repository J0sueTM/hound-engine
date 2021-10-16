/**
 * @file test/event/event.c
 * @author Josue Teodoro Moreira <teodoro.josue@protonmail.ch>
 * @date September 29, 2021
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

#include "../src/hound.h"

int
main
(
  void
)
{
  hnd_window_t *window = hnd_create_window("Hound Engine Event Test",
                                           (hnd_vector){ 0, 0 },
                                           (hnd_vector){ 800, 600 },
                                           HND_WINDOW_DECORATION_ALL);

  hnd_set_renderer_clear_color(0.2f, 0.2f, 0.2f, 1.0f);

  hnd_event_t event;
#ifdef HND_WIN32
  hnd_set_window_event(window, &event);
#endif /* HND_WIN32 */

  while (window->running)
  {
    hnd_clear_render();
    hnd_poll_events(window, &event);

      if (event.type == HND_EVENT_KEY_PRESS)
        printf("Key pressed: %d\n", event.keyboard.pressed_key);
      else if (event.type == HND_EVENT_KEY_RELEASE)
        printf("Key released: %d\n", event.keyboard.released_key);
      else if (event.type == HND_EVENT_MOUSE_BUTTON_PRESS)
      {
        if (event.mouse.pressed_button == HND_MOUSE_BUTTON_MIDDLE_UP)
          printf("Mouse wheel scrolled up at: %.2f - %.2f\n", event.mouse.pressed_position[0], event.mouse.pressed_position[1]);
        else if (event.mouse.pressed_button == HND_MOUSE_BUTTON_MIDDLE_DOWN)
          printf("Mouse wheel scrolled down at: %.2f - %.2f\n", event.mouse.pressed_position[0], event.mouse.pressed_position[1]);
        else
          printf("Button %d pressed at: %.2f - %.2f\n", event.mouse.pressed_button, event.mouse.pressed_position[0], event.mouse.pressed_position[1]);
      }
      else if (event.type == HND_EVENT_MOUSE_BUTTON_RELEASE)
        printf("Button %d released at: %.2f - %.2f\n", event.mouse.released_button, event.mouse.released_position[0], event.mouse.released_position[1]);
      else if (event.type == HND_EVENT_MOUSE_MOVE)
        printf("Mouse moved at: %.2f : %.2f\n", event.mouse.position[0], event.mouse.position[1]);

    hnd_swap_renderer_buffers(&window->renderer);
  }

  hnd_destroy_window(window);

  return 0;
}
