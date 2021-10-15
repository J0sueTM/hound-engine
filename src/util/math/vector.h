/**
 * @file src/util/math/vector.h
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
 *
 * @note Every vector is a 4d vector. I could use a union, but it would just create the same
 * amount of memory (the biggest, in this case, a 4d vector).
 */

#ifndef __HND_VECTOR_H__
#define __HND_VECTOR_H__

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#include "../../core/core.h"

typedef float hnd_vector[4];

void
hnd_add_vector
(
  hnd_vector _left,
  hnd_vector _right
);

void
hnd_subtract_vector
(
  hnd_vector _left,
  hnd_vector _right
);

void
hnd_multiply_vector
(
  hnd_vector _left,
  hnd_vector _right
);

void
hnd_divide_vector
(
  hnd_vector _left,
  hnd_vector _right
);

void
hnd_print_vector
(
  hnd_vector _vector
);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __HND_VECTOR_H_- */
