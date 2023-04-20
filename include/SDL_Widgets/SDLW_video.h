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
 *  \file SDLW_video.h
 *
 *  Header file for SDL_Widgets video functions.
 **/

#ifndef SDLW_video_h_
#define SDLW_video_h_

#include "SDLW_defines.h"


typedef struct SDLW_DeviceContext
{
    SDL_Window* Window;
    enum { SDLW_DC_Surface   = 1,          /**< Surface and fallback device context */
           SDLW_DC_Renderer  = 0,          /**< Renderer */
           SDLW_DC_GLContext = 2,          /**< OpenGL */
           SDLW_DC_VkSurface = 3 } Type;   /**< Vulkan Surface */
    union
    {
      SDL_Surface* Surface;
      struct
      {
          SDL_Renderer* handle;
          SDL_string    driverName;
          Uint32        flags;
      } Renderer;
      SDL_GLContext* GLContext;
     #ifdef HAVE_VULKAN_H
      struct
      {
          VkSurfaceKHR* surface;
          VkInstance    instance;
      } Vulkan;
     #endif
    };
} SDLW_DeviceContext;

bool SDLWAPI SDLW_GetDeviceContext(SDLW_DeviceContext DeviceContext);
bool SDLWAPI SDLW_SetDeviceContext(SDLW_DeviceContext DeviceContext);
bool SDLWAPI SDLW_QueryDeviceContext(SDLW_DeviceContext DeviceContext);

typedef struct SDLW_DisplayInfo
{
    struct
    {
        SDL_string  name;
        int         index;
    } VideoDriver;
    struct
    {
        int              index;
        SDL_DisplayMode  mode, currentMode, closestMode;
        int              modeIndex;
        SDL_Rect         bounds, usableBounds;
        struct DPI
        {
            float  diagonal, horizontal, vertical;
        } DPI;
        SDL_DisplayOrientation  orientation;
    } Display;
} SDLW_DisplayInfo;

int SDLWAPI SDLW_EnumVideoDrivers(int (fnEnumVideoDrivers)(int index, SDL_string));
int SDLWAPI SDLW_EnumVideoDisplays(int (fnEnumVideoDisplays)(SDLW_DisplayInfo* displayInfo));


int SDLWAPI SDLW_DrawRect(SDL_Renderer *renderer, SDL_Rect* rect, SDL_Color* color);
#endif // SDLW_video_h_
