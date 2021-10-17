/**
 * @file src/util/math/vector.c
 * @author Josue Teodoro Moreira <teodoro.josue@protonmail.ch>
 * @date October 15, 2021
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

#include "vector.h"

void
hnd_copy_vector
(
  hnd_vector_t _source,
  hnd_vector_t _destination
)
{
  for (int i = 0; i < 4; ++i)
    _destination[i] = _source[i];
}

void
hnd_add_vector
(
  hnd_vector_t _left,
  hnd_vector_t _right
)
{
  for (int i = 0; i < 4; ++i)
    _left[i] += _right[i];
}

void
hnd_subtract_vector
(
  hnd_vector_t _left,
  hnd_vector_t _right
)
{
  for (int i = 0; i < 4; ++i)
    _left[i] -= _right[i];
}

void
hnd_multiply_vector
(
  hnd_vector_t _left,
  hnd_vector_t _right
)
{
  for (int i = 0; i < 4; ++i)
    _left[i] *= _right[i];
}

void
hnd_divide_vector
(
  hnd_vector_t _left,
  hnd_vector_t _right
)
{
  for (int i = 0; i < 4; ++i)
    _left[i] /= _right[i];
}

void
hnd_print_vector
(
  hnd_vector_t _vector
)
{
  for (int i = 0; i < 4; ++i)
    printf("%.2f ", _vector[i]);
  printf("\n");
}
