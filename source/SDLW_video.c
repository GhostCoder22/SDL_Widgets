#include <SDL_Widgets/SDLW_defines.h>
#include <SDL_Widgets/SDLW_error.h>
#include <SDL_Widgets/SDLW_video.h>
#include "SDLW_internals.h"

extern SDLW_ m_SDLW;


bool SDLWAPI SDLW_SetDeviceContext(SDLW_DeviceContext DeviceContext)
{
  SDLW_ClearError();
  if (m_SDLW.fWasInit == false)
  {
    SDLW_SetError(SDLW_E_NOT_INIT);
    return false;
  }

  if (DeviceContext->Window == NULL)
  {
    SDLW_SetError(SDLW_E_INVALID_WINDOW);
    return NULL;
  }

  if (DeviceContext.Type == SDLW_DC_VkSurface)
  {
   #ifndef HAVE_VULKAN_H
    SDLW_SetError(SDLW_E_NOT_IMPLEMENTED);
    return false;
   #else
    memset(&m_SDLW.DeviceContext, 0, sizeof(SDLW_DeviceContext));
    m_SDLW.Info.DeviceContext.Vulkan.Surface  = DeviceContext->Vulkan.Surface;
    m_SDLW.Info.DeviceContext.Vulkan.Instance = DeviceContext->Vulkan.Instance;
    return true;
   #endif
  }

  memset(&m_SDLW.Info.DeviceContext, 0, sizeof(SDLW_DeviceContext));
  switch (DeviceContext->Type)
  {
    case SDLW_DC_Renderer:
    {
      if (DeviceContext->Renderer.handle == NULL)
      {
        SDLW_SetError(SDLW_E_INVALID_PARAMETER);
        return false;
      }

      SDL_Renderer* renderer = SDL_CreateRenderer(DeviceContext->Window,
                                                  DeviceContext->Renderer.driverName,
                                                  DeviceContext->Renderer.flags);
      if (renderer == NULL)
      {
        m_SDLW.Info.DeviceContext.Surface = SDL_GetWindowSurface(DeviceContext->Window);
        if (m_SDLW.Info.DeviceContext.Surface == NULL)
        {
          SDLW_SetError(SDLW_E_UNKNOWN_ERROR);
          return false;
        }
        m_SDLW.Info.DeviceContext.Type = SDLW_DC_Surface;
      }
      else
        m_SDLW.Info.DeviceContext.Renderer.handle = renderer;
    }
    break;

    case SDLW_DC_GLContext:
    {
      if (SDL_GetWindowFlags(DeviceContext->Window) &~ SDL_WINDOW_OPENGL)
        SDLW_SetError(SDLW_E_INVALID_PARAMETER);

      }
    }

    case SDLW_DC_Surface:
    case SDLW_DC_VkSurface:
    {
     /* Since 'SDLW_DC_VkSurface' is already handled above,
        this is just a placeholder to prevent 'end of label' errors. */
    }
    break;
  }
  m_SDLW.Info.DeviceContext.Type = DeviceContext->Type;
  return true;
}

bool SDLW_GetDeviceContext(SDLW_DeviceContext DeviceContext)
{
  SDLW_ClearError();
  if (m_SDLW.fWasInit == false)
  {
    SDLW_SetError(SDLW_E_NOT_INIT);
    return false;
  }

  DeviceContext.Type = m_SDLW.Info.DeviceContext.Type;
  switch (m_SDLW.Info.DeviceContext.Type)
  {
    case SDLW_DC_Renderer:  DeviceContext.Renderer  = m_SDLW.Info.DeviceContext.Renderer;  break;
    case SDLW_DC_Surface:   DeviceContext.Surface   = m_SDLW.Info.DeviceContext.Surface;   break;
    case SDLW_DC_GLContext: DeviceContext.GLContext = m_SDLW.Info.DeviceContext.GLContext; break;
    case SDLW_DC_VkSurface:
    {
     #ifdef HAVE_VULKAN_H
      DrawingContext.Vulkan.Surface  = m_SDLW.Info.DrawingContext.DrawingContext.Vulkan.Surface;
      DrawingContext.Vulkan.Instance = m_SDLW.Info.DrawingContext.DrawingContext.Vulkan.Instance;
     #endif
    }
  }
  return true;
}

bool SDLW_QueryDeviceContext(SDLW_DeviceContext DeviceContext)
{
  SDLW_ClearError();
  if (m_SDLW.fWasInit == false)
  {
    SDLW_SetError(SDLW_E_NOT_INIT);
    return false;
  }

  if (DeviceContext.Type == m_SDLW.Info.DeviceContext.Type)
    return true;
  return false;
}


int SDLW_EnumVideoDrivers(int (fnEnumVideoDrivers)(int index, SDL_string))
{
  SDLW_ClearError();
  if (m_SDLW.fWasInit == false)
  {
    SDLW_SetError(SDLW_E_NOT_INIT);
    return -2;
  }

  if (fnEnumVideoDrivers == NULL)
  {
    SDLW_SetError(SDLW_E_INVALID_PARAMETER);
    return 3;
  }

  int videoDrivers = SDL_GetNumVideoDrivers();
  if (videoDrivers <= 0)
    return videoDrivers;

  for (int i = 1; i <= videoDrivers; i++)
  {
    int result = ((fnEnumVideoDrivers)(i, SDL_GetVideoDriver(i)));
    if (result < 0)
      return result;
  }
  return -4;
}

int SDLW_EnumVideoDisplays(int (fnEnumVideoDisplays)(SDLW_DisplayInfo* displayInfo))
{
  SDLW_ClearError();
  if (m_SDLW.fWasInit == false)
  {
    SDLW_SetError(SDLW_E_NOT_INIT);
    return -2;
  }

  if (fnEnumVideoDisplays == NULL)
  {
    SDLW_SetError(SDLW_E_INVALID_PARAMETER);
    return 3;
  }

  int videoDisplays = SDL_GetNumVideoDisplays();
  if (videoDisplays <= 0)
    return videoDisplays;

  for (int i = 1; i <= videoDisplays; i++)
  {
    int displayModes = SDL_GetNumDisplayModes(i);
    if (displayModes <= 0)
      return displayModes;

    for (int m = 1; m <= displayModes; m++)
    {
      SDLW_DisplayInfo m_displayInfo;
      memset(&m_displayInfo, 0, sizeof(SDLW_DisplayInfo));

      m_displayInfo.VideoDriver.name = SDL_GetDisplayName(i);
      m_displayInfo.VideoDriver.index = i;
      m_displayInfo.Display.index = 1;
      SDL_GetDisplayMode(i, m, &m_displayInfo.Display.mode);
      m_displayInfo.Display.modeIndex = m;
      SDL_GetCurrentDisplayMode(i, &m_displayInfo.Display.currentMode);
      SDL_GetClosestDisplayMode(i, &m_displayInfo.Display.mode, &m_displayInfo.Display.closestMode);
      SDL_GetDisplayBounds(i, &m_displayInfo.Display.bounds);
      SDL_GetDisplayUsableBounds(i, &m_displayInfo.Display.usableBounds);
      SDL_GetDisplayDPI(i, &m_displayInfo.Display.DPI.diagonal,
                           &m_displayInfo.Display.DPI.horizontal,
                           &m_displayInfo.Display.DPI.vertical);
      m_displayInfo.Display.orientation = SDL_GetDisplayOrientation(i);

      int result = ((fnEnumVideoDisplays)(&m_displayInfo));
      memset(&m_displayInfo, 0, sizeof(SDLW_DisplayInfo));
      if (result < 0)
        return result;
    }
  }
  return -4;
}



int SDLW_DrawRect(SDL_Renderer *renderer, SDL_Rect* rect, SDL_Color* color)
{
  SDLW_ClearError();
  if (m_SDLW.fWasInit == false)
  {
    SDLW_SetError(SDLW_E_NOT_INIT);
    return -2;
  }

  if (renderer == NULL)
  {
    SDLW_SetError(SDLW_E_INVALID_PARAMETER);
    return 0;
  }

  if (color != NULL)
    SDL_SetRenderDrawColor(renderer, color->r, color->g, color->b, color->a);
	return SDL_RenderFillRect(renderer, rect);
}
