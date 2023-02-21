#include "SDLW_kernel_internals.h"
#include <SDL_Widgets/SDLW_kernel.h>
#include <SDL_Widgets/SDLW_error.h>

SDLW_ m_SDLW;

void SDLW_Init(SDLW* info)
{
  SDLW_SetError(0);
  if (m_SDLW.fWasInit == false)
  {
    if (info == NULL || info->AppName == NULL)
      SDLW_SetError(SDLW_E_INVALID_PARAMETER);
    else
    {
      memset(&m_SDLW, 0, sizeof(SDLW));
      SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | info->flags);
      m_SDLW.Info.AppName = info->AppName;
      m_SDLW.Info.DeviceContext.Type = info->DeviceContext.Type;
      m_SDLW.fWasInit = true;
    }
  }
}

bool SDLW_WasInit(void)
{
  return m_SDLW.fWasInit;
}

SDL_string SDLWAPI SDLW_GetAppName(void)
{
  SDLW_ClearError();
  if (m_SDLW.fWasInit == false)
  {
    SDLW_SetError(SDLW_E_NOT_INIT);
    return NULL;
  }
  return m_SDLW.Info.AppName;
}

bool SDLW_Quit(void)
{
  SDLW_ClearError();
  if (m_SDLW.fWasInit == false)
  {
    SDLW_SetError(SDLW_E_NOT_INIT);
    return false;
  }

  struct SDLW_Widget* widget = m_SDLW.WidgetTable;
  while (widget != NULL)
  {
    struct SDLW_Widget* tempWidget = widget;
    widget = widget->pNext;
    free(tempWidget); tempWidget = NULL;
    m_SDLW.Widgets--;
  }

  if (m_SDLW.Widgets > 0)
  {
    SDLW_SetError(SDLW_E_MEMORY_ERROR);
    return false;
  }

  struct SDLW_Class_* widgetClass = m_SDLW.ClassTable;
  while (widgetClass != NULL)
  {
    struct SDLW_Class_* tempClass = widgetClass;
    widgetClass = widgetClass->pNext;
    free(tempClass); tempClass = NULL;
    m_SDLW.Classes--;
  }

  if (m_SDLW.Classes > 0)
  {
    SDLW_SetError(SDLW_E_MEMORY_ERROR);
    return false;
  }

  memset(&m_SDLW, 0, sizeof(SDLW));
  m_SDLW.fWasInit = false;
  SDL_Quit();
  return true;
}


bool SDLW_SetDeviceContext(SDLW_DeviceContext DeviceContext)
{
  SDLW_ClearError();
  if (m_SDLW.fWasInit == false)
  {
    SDLW_SetError(SDLW_E_NOT_INIT);
    return false;
  }

  if (DeviceContext.Type == SDLW_DC_VkSurface)
  {
   #ifndef HAVE_VULKAN_H
    SDLW_SetError(SDLW_E_NOT_IMPLEMENTED);
    return false;
   #else
    memset(&m_SDLW.DeviceContext, 0, sizeof(SDLW_DeviceContext));
    m_SDLW.Info.DeviceContext.Vulkan.Surface  = DeviceContext.Vulkan.Surface;
    m_SDLW.Info.DeviceContext.Vulkan.Instance = DeviceContext.Vulkan.Instance;
    return true;
   #endif
  }

  memset(&m_SDLW.Info.DeviceContext, 0, sizeof(SDLW_DeviceContext));
  m_SDLW.Info.DeviceContext.Type = DeviceContext.Type;
  switch (DeviceContext.Type)
  {
    case SDLW_DC_Renderer:  m_SDLW.Info.DeviceContext.Renderer  = DeviceContext.Renderer;  break;
    case SDLW_DC_Surface:   m_SDLW.Info.DeviceContext.Surface   = DeviceContext.Surface;   break;
    case SDLW_DC_GLContext: m_SDLW.Info.DeviceContext.GLContext = DeviceContext.GLContext; break;
    case SDLW_DC_VkSurface:
    {
     /* Since 'SDLW_DC_VkSurface' is already handled above,
        this is just a placeholder to prevent 'end of label' errors. */
    }
    break;
  }
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





SDL_byte SDLW_RegisterClass(SDLW_Class* lpwc)
{
  SDLW_ClearError();
  if (m_SDLW.fWasInit == false)
  {
    SDLW_SetError(SDLW_E_NOT_INIT);
    return 0;
  }

  if (m_SDLW.Classes > 255)
  {
    SDLW_SetError(SDLW_E_TABLE_FULL);
    return 0;
  }

  if (lpwc == NULL || lpwc->procedure == NULL)
  {
    SDLW_SetError(SDLW_E_INVALID_PARAMETER);
    return 0;
  }

  struct SDLW_Class_* widgetClass = m_SDLW.ClassTable;
  while (widgetClass != NULL)
  {
    if (strcmp(widgetClass->info.className, lpwc->className) == 0)
    {
      SDLW_SetError(SDLW_E_ALREADY_EXISTS);
      return widgetClass->info.id;
    }
    widgetClass = widgetClass->pNext;
  }

  struct SDLW_Class_* newClass = (struct SDLW_Class_*) calloc(1, sizeof(struct SDLW_Class_));
  if (newClass == NULL)
  {
    SDLW_SetError(SDLW_E_MEMORY_ERROR);
    return 0;
  }

  newClass->pNext = NULL;

  newClass->info.className = lpwc->className;
  newClass->info.procedure = lpwc->procedure;
  m_SDLW.Classes++;
  newClass->info.id     = m_SDLW.Classes;
  newClass->info.styles = lpwc->styles;
  newClass->info.data   = lpwc->data;

  newClass->pNext = NULL;
  if (m_SDLW.ClassTable == NULL)
    m_SDLW.ClassTable = newClass;
  else
  {
    struct SDLW_Class_* widgetClass = m_SDLW.ClassTable;
    if (widgetClass->pNext != NULL)
      widgetClass = widgetClass->pNext;
    widgetClass->pNext = newClass;
  }
  printf("1\n");
  return newClass->info.id;
}



bool SDLW_SetClassInfo
 (SDL_dword flags, SDL_string className, SDLW_Class* info)
{
  SDLW_SetError(SDLW_E_NOT_IMPLEMENTED);
  return 0;
}

SDL_byte SDLW_GetClassInfo(SDL_string className, SDLW_Class* info)
{
  SDLW_ClearError();
  if (m_SDLW.fWasInit == false)
  {
    SDLW_SetError(SDLW_E_NOT_INIT);
    return 0;
  }

  if (info == NULL)
  {
    SDLW_SetError(SDLW_E_INVALID_PARAMETER);
    return 0;
  }

  if (m_SDLW.Classes == 0)
  {
    SDLW_SetError(SDLW_E_TABLE_EMPTY);
    return 0;
  }

  struct SDLW_Class_* widgetClass = m_SDLW.ClassTable;
  while (widgetClass->pNext != NULL)
  {
    if (strcmp(widgetClass->info.className, className) == 0)
    {
      info->className = className;
      info->procedure = widgetClass->info.procedure;
      info->id        = widgetClass->info.id;
      info->styles    = widgetClass->info.styles;
      info->data      = widgetClass->info.data;
      return widgetClass->info.id;
    }
    widgetClass = widgetClass->pNext;
  }
  SDLW_SetError(SDLW_E_INVALID_CLASS);
  return 0;
}

bool SDLW_UnregisterClass(SDL_string className)
{
  SDLW_ClearError();
  if (className == NULL)
  {
    SDLW_SetError(SDLW_E_INVALID_PARAMETER);
    return 0;
  }

  struct SDLW_Class_* widgetClass = (struct SDLW_Class_*) calloc(1, sizeof(struct SDLW_Class_));
  while (widgetClass->pNext != NULL)
  {
    if (strcmp(widgetClass->info.className, className) == 0)
    {
      struct SDLW_Class_* temp = widgetClass;
      widgetClass = widgetClass->pNext;
      free(temp); temp = NULL;
      m_SDLW.Classes--;
      return true;
    }
    widgetClass = widgetClass->pNext;
  }
  SDLW_SetError(SDLW_E_INVALID_CLASS);
  return false;
}





SDLW_Widget* SDLW_CreateWidget(SDLW_WidgetInfo* lpwc)
{
  SDLW_ClearError();
  if (m_SDLW.fWasInit == false)
  {
    SDLW_SetError(SDLW_E_NOT_INIT);
    return false;
  }

  if (m_SDLW.Widgets > 65535)
  {
    SDLW_SetError(SDLW_E_TABLE_FULL);
    return false;
  }

  if (lpwc == NULL)
  {
    SDLW_SetError(SDLW_E_INVALID_PARAMETER);
    return NULL;
  }

  /*if (lpwc->widget_data == NULL)
  {
    SDLW_SetError(SDLW_E_INVALID_DATA);
    return NULL;
  }*/

  struct SDLW_Class_* widgetClass = m_SDLW.ClassTable;
  while (widgetClass != NULL)
  {
    printf("\%s\"\n", widgetClass->info.className);
    if (strcmp(widgetClass->info.className, lpwc->className) == 0)
    {
      struct SDLW_Widget* widget = m_SDLW.WidgetTable;
      if (m_SDLW.Widgets > 1)
      {
        if (widget->pNext != NULL)
          widget = widget->pNext;
      }

      struct SDLW_Widget* newWidget = (struct SDLW_Widget*) calloc(1, sizeof(struct SDLW_Widget));
      if (newWidget == NULL)
      {
        SDLW_SetError(SDLW_E_MEMORY_ERROR);
        return NULL;
      }

      newWidget->pNext = NULL;
      newWidget->pPrev = NULL;

      newWidget->Class = widgetClass;
      m_SDLW.Widgets++;
      newWidget->id          = m_SDLW.Widgets;
      newWidget->styles      = lpwc->styles;
      newWidget->rect.x      = lpwc->x;
      newWidget->rect.y      = lpwc->y;
      newWidget->rect.w      = lpwc->width;
      newWidget->rect.h      = lpwc->height;
      newWidget->parent      = lpwc->parent;
      newWidget->widget_data = lpwc->widget_data;
      newWidget->user_data   = lpwc->user_data;

      newWidget->pNext = NULL;
      if (m_SDLW.WidgetTable == NULL)
        m_SDLW.WidgetTable = newWidget;
      else
      {
        struct SDLW_Widget* widget = m_SDLW.WidgetTable;
        if (widget->pNext != NULL)
          widget = widget->pNext;
        newWidget->pPrev = widget;
        widget->pNext = newWidget;
      }
      return newWidget;
    }
    widgetClass = widgetClass->pNext;
  }
  SDLW_SetError(SDLW_E_INVALID_CLASS);
  return NULL;
}

SDLW_WidgetID SDLW_GetWidgetInfo(SDLW_Widget* widget, SDLW_WidgetInfo* info)
{
  SDLW_ClearError();
  if (widget == NULL || info == NULL)
  {
    SDLW_SetError(SDLW_E_INVALID_PARAMETER);
    return 0;
  }

  if (widget->id == 0)
  {
    SDLW_SetError(SDLW_E_INVALID_WIDGET);
    return 0;
  }

  info->className   = widget->Class->info.className;
  info->styles      = widget->styles;
  info->id          = widget->id;
  info->x           = widget->rect.x; info->y      = widget->rect.y;
  info->width       = widget->rect.w; info->height = widget->rect.h;
  info->parent      = widget->parent;
  info->widget_data = widget->widget_data;
  info->user_data   = widget->user_data;

  return widget->id;
}

bool SDLW_DestroyWidget(SDLW_Widget* widget)
{
  SDLW_ClearError();
  if (widget == NULL)
  {
    SDLW_SetError(SDLW_E_INVALID_PARAMETER);
    return 0;
  }

  if (widget->id == 0)
  {
    SDLW_SetError(SDLW_E_INVALID_WIDGET);
    return 0;
  }

  struct SDLW_Widget* ppWidget = m_SDLW.WidgetTable;
  while (ppWidget != NULL)
  {
    if (widget->id == ppWidget->id)
    {
      SDLW_Widget* temp = ppWidget;
      ppWidget = ppWidget->pNext;
      free(temp); temp = NULL;
      m_SDLW.Widgets--;
      return true;
    }
    ppWidget = ppWidget->pNext;
  }
  SDLW_SetError(SDLW_E_INVALID_WIDGET);
  return false;
}


SDL_string SDLW_GetWidgetClass(SDLW_Widget* widget)
{
  SDLW_ClearError();
  if (widget == NULL)
  {
    SDLW_SetError(SDLW_E_INVALID_PARAMETER);
    return 0;
  }

  if (widget->id == 0)
  {
    SDLW_SetError(SDLW_E_INVALID_WIDGET);
    return 0;
  }

  return widget->Class->info.className;
}

SDLW_WidgetID SDLW_GetWidgetID(SDLW_Widget* widget)
{
  SDLW_ClearError();
  if (widget == NULL)
  {
    SDLW_SetError(SDLW_E_INVALID_PARAMETER);
    return 0;
  }

  if (widget->id == 0)
  {
    SDLW_SetError(SDLW_E_INVALID_WIDGET);
    return 0;
  }

  return widget->id;
}


SDLW_Widget* SDLW_GetWidgetParent(SDLW_Widget* widget)
{
  SDLW_ClearError();
  if (widget == NULL)
  {
    SDLW_SetError(SDLW_E_INVALID_PARAMETER);
    return NULL;
  }

  if (widget->id == 0)
  {
    SDLW_SetError(SDLW_E_INVALID_WIDGET);
    return NULL;
  }

  return widget->parent;
}

bool SDLW_CallWidgetProc(SDLW_Widget* widget)
{
  SDLW_ClearError();
  if (widget == NULL)
  {
    SDLW_SetError(SDLW_E_INVALID_PARAMETER);
    return 0;
  }

  if (widget->id == 0)
  {
    SDLW_SetError(SDLW_E_INVALID_WIDGET);
    return 0;
  }

  SDLW_Event event;
  event.widget = widget;
  return (widget->Class->info.procedure)(&event);
}
