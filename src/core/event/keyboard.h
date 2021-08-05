/**
 * @file src/core/event/keyboard.h
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
 * @note Taken from mostly from GLFW's documentation:
 * https://www.glfw.org/docs/latest/group__keys.html#ga50391730e9d7112ad4fd42d0bd1597c1
 */

#ifndef HND_KEYBOARD_H
#define HND_KEYBOARD_H

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/**
 * Keyboard map based on us qwerty
 *
 * @note Run xev.
 *
 * @note I use dvorak, but this helps when developing games
 * since we won't need to worry since qwerty is mainstream.
 */
typedef enum hnd_keyboard_key
{
  HOUND_KEY_UNKNOWN = 0,

  /* function row */
  HND_KEY_F1  = 67,
  HND_KEY_F2  = 68,
  HND_KEY_F3  = 69,
  HND_KEY_F4  = 70,
  HND_KEY_F5  = 71,
  HND_KEY_F6  = 72,
  HND_KEY_F7  = 73,
  HND_KEY_F8  = 74,
  HND_KEY_F9  = 75,
  HND_KEY_F10 = 76,
  HND_KEY_F11 = 95,
  HND_KEY_F12 = 96,

  /* number row */
  HND_KEY_1     = 10,
  HND_KEY_2     = 11,
  HND_KEY_3     = 12,
  HND_KEY_4     = 13,
  HND_KEY_5     = 14,
  HND_KEY_6     = 15,
  HND_KEY_7     = 16,
  HND_KEY_8     = 17,
  HND_KEY_9     = 18,
  HND_KEY_0     = 19,
  HND_KEY_MINUS = 20,
  HND_KEY_EQUAL = 21,

  /* upper row */
  HND_KEY_Q = 24,
  HND_KEY_W = 25,
  HND_KEY_E = 26,
  HND_KEY_R = 27,
  HND_KEY_T = 28,
  HND_KEY_Y = 29,
  HND_KEY_U = 30,
  HND_KEY_I = 31,
  HND_KEY_O = 32,
  HND_KEY_P = 33,

  /* home row */
  HND_KEY_A         = 38,
  HND_KEY_S         = 39,
  HND_KEY_D         = 40,
  HND_KEY_F         = 41,
  HND_KEY_G         = 42,
  HND_KEY_H         = 43,
  HND_KEY_J         = 44,
  HND_KEY_K         = 45,
  HND_KEY_L         = 46,
  HND_KEY_SEMICOLON = 46,
  
  /* lower row */
  HND_KEY_Z      = 52,
  HND_KEY_X      = 53,
  HND_KEY_C      = 54,
  HND_KEY_V      = 55,
  HND_KEY_B      = 56,
  HND_KEY_N      = 57,
  HND_KEY_M      = 58,
  HND_KEY_COMMA  = 59,
  HND_KEY_PERIOD = 60,
  HND_KEY_SLASH  = 61,

  /* left column */
  HND_KEY_ESC          = 9,
  HND_KEY_APOSTROPHE   = 48,
  HND_KEY_TAB          = 23,
  HND_KEY_CAPS         = 66,
  HND_KEY_LEFT_SHIFT   = 50,
  HND_KEY_LEFT_CONTROL = 37,

  /* right column */
  HND_KEY_BACKSPACE = 22,
  HND_KEY_BACKSLASH = 52,
  HND_KEY_ENTER = 36,
  HND_KEY_RIGHT_SHIFT = 62,
  HND_KEY_RIGHT_CONTROL = 105,

  /* bottom row */
  HND_KEY_LEFT_SUPER = 133,
  HND_KEY_LEFT_META = 64,
  HND_KEY_SPACE = 64,
  HND_KEY_RIGHT_META = 108,
  HND_KEY_MENU = 135,
  
  /* arrow keys */
  HND_KEY_UP    = 111,
  HND_KEY_LEFT  = 113,
  HND_KEY_RIGHT = 114,
  HND_KEY_DOWN  = 116,

  /* with shift */
  HND_KEY_LEFT_BRACKET = 34,
  HND_KEY_RIGHT_BRACKET = 35,
  HND_KEY_GRAVE_ACCENT = 49,

  /* etc */
  HND_KEY_PAUSE     = 127,
  HND_KEY_INSERT    = 118,
  HND_KEY_DELETE    = 119,
  HND_KEY_PAGE_UP   = 112,
  HND_KEY_PAGE_DOWN = 117,
  HND_KEY_HOME      = 110,
  HND_KEY_END       = 115
} hnd_keyboard_key;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* HND_KEYBOARD_H */
