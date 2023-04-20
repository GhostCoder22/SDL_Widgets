/*
  SDL Widgets
  Copyright (C) 2022-2023 Ghost Coder (@ghostcoder22@gmail.com)

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/

/**
 *  \file SDLW_error.h
 *
 *  Simple error message routines for SDL_Widgets.
 **/

#ifndef SDLW_error_h_
#define SDLW_error_h_

#ifdef __cplusplus
extern "C" {
#endif

#include "SDLW_defines.h"

#define SDLW_E_OK                 0L    /**< No error. */
#define SDLW_E_UNKNOWN_ERROR      1L    /**< Unknown error. */
#define SDLW_E_NOT_INIT           2L    /**< SDL_Widgets not initialized. */
#define SDLW_E_NOT_IMPLEMENTED    3L    /**< Function not implemented. */
#define SDLW_E_INVALID_WINDOW     4L    /**< Invalid SDL window. */
#define SDLW_E_INVALID_PARAMETER  5L    /**< Invalid function parameter. */
#define SDLW_E_MEMORY_ERROR       6L    /**< Memory allocation error. */
#define SDLW_E_INVALID_CLASS      7L    /**< Invalid widget/document class.*/
#define SDLW_E_INVALID_WIDGET     8L    /**< Invalid widget. / Widget not found! */
#define SDLW_E_INVALID_DATA       9L    /**< Invalid data. */
#define SDLW_E_ALREADY_EXISTS    10L    /**< Already exists. */
#define SDLW_E_TABLE_EMPTY       11L    /**< Data table empty. */
#define SDLW_E_TABLE_FULL        12L    /**< Data table full. */
#define SDLW_E_CMDLINE_ERROR     13L    /**< Command line retrieval error. */

SDL_dword SDLWAPI SDLW_SetError(SDL_dword errorCode);
SDL_dword SDLWAPI SDLW_GetError(void);
void      SDLWAPI SDLW_ClearError(void);

char*     SDLWAPI SDLW_ErrorToString(SDL_string message, ...);
char*     SDLWAPI SDLW_ErrorToStringEx(SDL_dword errorCode, SDL_string message, ...);

#ifdef __cplusplus
}
#endif
#endif // SDLW_error_h_
