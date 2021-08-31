/**
 * @file src/core/core.h
 * @author Josue Teodoro Moreira <teodoro.josue@protonmail.ch>
 * @date July 27, 2021
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

#ifndef __HND_CORE_H__
#define __HND_CORE_H__

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef HND_WIN32
#include <windows.h>
#endif /* _WIN32 */
#include "debug.h"

#define HND_NAME "Hound"
#define HND_VERSION  "0.1.0"
#define HND_VERSION_MAJOR 0
#define HND_VERSION_MINOR 1
#define HND_VERSION_PATCH 0

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __HND_CORE_H__ */
