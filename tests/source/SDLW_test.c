#include <SDL_Widgets.h>

SDL_Widget* SDLW_test_CreateWidget(const char* title)
{
  SDL_Widget* widget = SDLW_CreateWidget(title);
  const char* result = NULL;

  if (widget == NULL)
    result = "NULL (UNSUCCESSFUL)";
  else
    result = "a non-NULL value (SUCCESSFUL)";
  printf("SDLW_CreateWidget(\"%s\") returned %s\n", title, result);
  return widget;
}

const char* SDLW_test_SetWidgetTitle(SDL_Widget* widget, const char* newTitle)
{
  const char* oldTitle = SDLW_SetWidgetTitle(widget, newTitle);
  const char* result = NULL;

  if (oldTitle == NULL)
    result = "<NULL> (UNSUCESSFUL)";
  else
    result = oldTitle;
  printf("SDLW_SetWidgetTitle(SDL_Widget* widget, \"%s\") returned \"%s\"\n", newTitle, result);
  return result;
}

const char* SDLW_test_GetWidgetTitle(SDL_Widget* widget)
{
  const char* title = SDLW_GetWidgetTitle(widget);
  const char* result = NULL;

  if (title == NULL)
    result = "<NULL> (UNSUCESSFUL)";
  else
    result = title;
  printf("SDLW_GetWidgetTitle(SDL_Widget* widget) returned \"%s\"\n", result);
  return result;
}

bool SDLW_test_DestroyWidget(SDL_Widget* widget)
{
  bool result = SDLW_DestroyWidget(widget);
  const char* result_s = NULL;

  if (!result)
    result_s = "FALSE";
  else
    result_s = "TRUE";
  printf("SDLW_DestroyWidget(SDL_Widget* widget) returned %s\n", result_s);
  return result;
}


int main(void)
{
  SDL_Widget* widget1 = SDLW_test_CreateWidget("One");
  SDL_Widget* widget2 = SDLW_test_CreateWidget("Five");
  SDL_Widget* widget3 = SDLW_test_CreateWidget("Three");

  printf("\n");
  system("PAUSE");
  printf("\n\n");

  SDLW_test_GetWidgetTitle(widget1);
  SDLW_test_GetWidgetTitle(widget2);
  SDLW_test_GetWidgetTitle(widget3);
  SDLW_test_SetWidgetTitle(widget2, "Two");
  SDLW_test_GetWidgetTitle(widget2);
  SDLW_test_GetWidgetTitle(widget1);

  printf("\n");
  system("PAUSE");
  printf("\n\n");


  SDLW_test_DestroyWidget(widget3);
  SDLW_test_DestroyWidget(widget2);
  SDLW_test_DestroyWidget(widget1);

  printf("\n");
  system("PAUSE");
  printf("\n\n");
  return 0;
}
