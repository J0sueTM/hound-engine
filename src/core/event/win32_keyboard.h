/**
 * @file src/core/event/win32_keyboard.h
 * @author Josue Teodoro Moreira <teodoro.josue@protonmail.ch>
 * @date October 13, 2021
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
 * @note https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
 */

#ifndef __HND_WIN32_KEYBOARD_H__
#define __HND_WIN32_KEYBOARD_H__

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#define HND_KEY_UNKNOWN 0

/* @note Function row */
#define HND_KEY_F1  VK_F1
#define HND_KEY_F2  VK_F2
#define HND_KEY_F3  VK_F3
#define HND_KEY_F4  VK_F4
#define HND_KEY_F5  VK_F5
#define HND_KEY_F6  VK_F6
#define HND_KEY_F7  VK_F7
#define HND_KEY_F8  VK_F8
#define HND_KEY_F9  VK_F9
#define HND_KEY_F10 VK_F10
#define HND_KEY_F11 VK_F11
#define HND_KEY_F12 VK_F12

/* @note Number row */
#define HND_KEY_1     0x31
#define HND_KEY_2     0x32
#define HND_KEY_3     0x33
#define HND_KEY_4     0x34
#define HND_KEY_5     0x35
#define HND_KEY_6     0x36
#define HND_KEY_7     0x37
#define HND_KEY_8     0x38
#define HND_KEY_9     0x39
#define HND_KEY_0     0x30
#define HND_KEY_MINUS VK_OEM_MINUS
#define HND_KEY_EQUAL VK_OEM_PLUS

/* @note Upper row */
#define HND_KEY_Q 0x51
#define HND_KEY_W 0x57
#define HND_KEY_E 0x45
#define HND_KEY_R 0x52
#define HND_KEY_T 0x54
#define HND_KEY_Y 0x59
#define HND_KEY_U 0x55
#define HND_KEY_I 0x49
#define HND_KEY_O 0x4F
#define HND_KEY_P 0x50

/* @note Home row */
#define HND_KEY_A         0x41
#define HND_KEY_S         0x53
#define HND_KEY_D         0x44
#define HND_KEY_F         0x46
#define HND_KEY_G         0x47
#define HND_KEY_H         0x48
#define HND_KEY_J         0x4A
#define HND_KEY_K         0x4B
#define HND_KEY_L         0x4C
#define HND_KEY_SEMICOLON VK_OEM_1

/* @note Lower row */
#define HND_KEY_Z      0x5A
#define HND_KEY_X      0x58
#define HND_KEY_C      0x43
#define HND_KEY_V      0x56
#define HND_KEY_B      0x42
#define HND_KEY_N      0x4D
#define HND_KEY_M      0x4E
#define HND_KEY_COMMA  VK_OEM_COMMA
#define HND_KEY_PERIOD VK_OEM_PERIOD
#define HND_KEY_SLASH  VK_OEM_2

/* @note Left column */
#define HND_KEY_ESC          VK_ESCAPE
#define HND_KEY_APOSTROPHE   VK_OEM_7
#define HND_KEY_TAB          VK_TAB
#define HND_KEY_CAPS         VK_CAPITAL
#define HND_KEY_LEFT_SHIFT   VK_LSHIFT
#define HND_KEY_LEFT_CONTROL VK_LCONTROL

/* @note Right column */
#define HND_KEY_BACKSPACE     VK_BACK
#define HND_KEY_BACKSLASH     VK_OEM_5
#define HND_KEY_ENTER         VK_RETURN
#define HND_KEY_RIGHT_SHIFT   VK_RSHIFT
#define HND_KEY_RIGHT_CONTROL VK_RCONTROL

/* @note Bottom row */
#define HND_KEY_LEFT_SUPER VK_LWIN
#define HND_KEY_LEFT_META  VK_LMENU
#define HND_KEY_SPACE      VK_SPACE
#define HND_KEY_RIGHT_META VK_RMENU
#define HND_KEY_MENU       VK_APPS

/* @note Arrow keys */
#define HND_KEY_UP    VK_UP
#define HND_KEY_LEFT  VK_LEFT
#define HND_KEY_RIGHT VK_RIGHT
#define HND_KEY_DOWN  VK_DOWN

/* @note With shift */
#define HND_KEY_LEFT_BRACKET  VK_OEM_4
#define HND_KEY_RIGHT_BRACKET VK_OEM_6
#define HND_KEY_GRAVE_ACCENT  VK_OEM_3

/* @note Etc */
#define HND_KEY_PAUSE     VK_PAUSE
#define HND_KEY_INSERT    VK_INSERT
#define HND_KEY_DELETE    VK_DELETE
#define HND_KEY_PAGE_UP   VK_PRIOR
#define HND_KEY_PAGE_DOWN VK_NEXT
#define HND_KEY_HOME      VK_HOME
#define HND_KEY_END       VK_END

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /*__HND_WIN32_KEYBOARD_H__ */
