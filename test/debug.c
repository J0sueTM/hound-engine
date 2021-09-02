/**
 * @file test/debug.c
 * @author Josue Teodoro Moreira <teodoro.josue@protonmail.ch>
 * @date September 02, 2021
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
  hnd_print_debug(HND_LOG, "Something went alright", HND_SUCCESS);
  hnd_print_debug(HND_WARNING, "Something needs attention", HND_FAILURE);
  hnd_print_debug(HND_ERROR, "Something went wrong", HND_FAILURE);
}
