#include "SDLW_internels.h"
#include <SDL_Widgets/SDLW_kernel.h>

SDL_Widget* SDLW_CreateWidget(const char* title)
{
  SDL_Widget* widget;

  if (title == NULL)
    return NULL;

  widget = (SDL_Widget*) calloc(1, sizeof(SDL_Widget));
  if (widget == NULL)
    return NULL;

  widget->title = title;
  return widget;
}

const char* SDLW_SetWidgetTitle(SDL_Widget* widget, const char* newTitle)
{
  if ((widget != NULL) && (newTitle != NULL))
  {
    const char* oldTitle = widget->title;
    widget->title = newTitle;
    return oldTitle;
  }
  return NULL;
}

const char* SDLW_GetWidgetTitle(SDL_Widget* widget)
{
  if (widget == NULL)
    return NULL;
  return widget->title;
}

bool SDLW_DestroyWidget(SDL_Widget* widget)
{
  if (widget == NULL)
    return false;
  free(widget);
  return true;
}

