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
 * @note These keycodes were extracted by running xev.
 * @note This will map every keyboard mapping to us qwerty.
 */

#ifndef __HND_KEYBOARD_H__ 
#define __HND_KEYBOARD_H__ 

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#define HND_KEY_UNKNOWN 0

/* @note Function row */
#define HND_KEY_F1  67
#define HND_KEY_F2  68
#define HND_KEY_F3  69
#define HND_KEY_F4  70
#define HND_KEY_F5  71
#define HND_KEY_F6  72
#define HND_KEY_F7  73
#define HND_KEY_F8  74
#define HND_KEY_F9  75
#define HND_KEY_F10 76
#define HND_KEY_F11 95
#define HND_KEY_F12 96

/* @note Number row */
#define HND_KEY_1     10
#define HND_KEY_2     11
#define HND_KEY_3     12
#define HND_KEY_4     13
#define HND_KEY_5     14
#define HND_KEY_6     15
#define HND_KEY_7     16
#define HND_KEY_8     17
#define HND_KEY_9     18
#define HND_KEY_0     19
#define HND_KEY_MINUS 20
#define HND_KEY_EQUAL 21

/* @note Upper row */
#define HND_KEY_Q 24
#define HND_KEY_W 25
#define HND_KEY_E 26
#define HND_KEY_R 27
#define HND_KEY_T 28
#define HND_KEY_Y 29
#define HND_KEY_U 30
#define HND_KEY_I 31
#define HND_KEY_O 32
#define HND_KEY_P 33

/* @note Home row */
#define HND_KEY_A         38
#define HND_KEY_S         39
#define HND_KEY_D         40
#define HND_KEY_F         41
#define HND_KEY_G         42
#define HND_KEY_H         43
#define HND_KEY_J         44
#define HND_KEY_K         45
#define HND_KEY_L         46
#define HND_KEY_SEMICOLON 46

/* @note Lower row */
#define HND_KEY_Z      52
#define HND_KEY_X      53
#define HND_KEY_C      54
#define HND_KEY_V      55
#define HND_KEY_B      56
#define HND_KEY_N      57
#define HND_KEY_M      58
#define HND_KEY_COMMA  59
#define HND_KEY_PERIOD 60
#define HND_KEY_SLASH  61

/* @note Left column */
#define HND_KEY_ESC          9
#define HND_KEY_APOSTROPHE   48
#define HND_KEY_TAB          23
#define HND_KEY_CAPS         66
#define HND_KEY_LEFT_SHIFT   50
#define HND_KEY_LEFT_CONTROL 37

/* @note Right column */
#define HND_KEY_BACKSPACE     22
#define HND_KEY_BACKSLASH     52
#define HND_KEY_ENTER         36
#define HND_KEY_RIGHT_SHIFT   62
#define HND_KEY_RIGHT_CONTROL 105

/* @note Bottom row */
#define HND_KEY_LEFT_SUPER 133
#define HND_KEY_LEFT_META  64
#define HND_KEY_SPACE      64
#define HND_KEY_RIGHT_META 108
#define HND_KEY_MENU       135

/* @note Arrow keys */
#define HND_KEY_UP    111
#define HND_KEY_LEFT  113
#define HND_KEY_RIGHT 114
#define HND_KEY_DOWN  116

/* @note With shift */
#define HND_KEY_LEFT_BRACKET  34
#define HND_KEY_RIGHT_BRACKET 35
#define HND_KEY_GRAVE_ACCENT  49

/* @note Etc */
#define HND_KEY_PAUSE     127
#define HND_KEY_INSERT    118
#define HND_KEY_DELETE    119
#define HND_KEY_PAGE_UP   112
#define HND_KEY_PAGE_DOWN 117
#define HND_KEY_HOME      110
#define HND_KEY_END       115

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __HND_KEYBOARD_H__ */
