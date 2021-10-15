/**
 * @file test/vector.c
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

#include "../src/hound.h"

int
main
(
  void
)
{
  printf("-- 2D VECTORS --\n");

  hnd_vector test_vector;
  test_vector[0] = 25.0f;
  test_vector[1] = 50.0f;

  hnd_vector test_vector2;
  test_vector2[0] = 75.0f;
  test_vector2[1] = 50.0f;

  hnd_print_vector(test_vector);
  hnd_print_vector(test_vector2);

  printf("-- ADD --\n");
  hnd_add_vector(test_vector2, test_vector);
  hnd_print_vector(test_vector2);

  printf("-- SUB --\n");
  test_vector[0] = 88.0f;
  test_vector[1] = 23.0f;
  hnd_subtract_vector(test_vector2, test_vector);
  hnd_print_vector(test_vector2);

  printf("-- 3D VECTORS --\n");
  hnd_vector test_vector3;
  test_vector[0] = 50.0f;
  test_vector[1] = 25.0f;
  test_vector[2] = 73.0f;

  hnd_vector test_vector4;
  test_vector4[0] = 25.0f;
  test_vector4[1] = 12.0f;
  test_vector4[2] = 55.0f;

  hnd_print_vector(test_vector3);
  hnd_print_vector(test_vector4);

  printf("-- MULT --\n");
  hnd_multiply_vector(test_vector3, (hnd_vector){ 2.0f, 1.5f, 0.5f });
  hnd_print_vector(test_vector3);

  printf("-- 4D VECTOR --\n");
  hnd_vector test_vector5;
  test_vector5[0] = 4.0f;
  test_vector5[1] = 6.0f;
  test_vector5[2] = 10.0f;
  test_vector5[3] = 16.0f;

  hnd_print_vector(test_vector5);

  printf("-- DIV --\n");
  hnd_divide_vector(test_vector5, (hnd_vector){ 2.0f, 1.0f, 5.0f, 2.0f });
  hnd_print_vector(test_vector5);
}
