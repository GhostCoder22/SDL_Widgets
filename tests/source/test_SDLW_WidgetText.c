#ifdef WINDOWS
#include <windows.h>
#endif
#include <SDL_Widgets.h>

SDL_string test_SDLW_ErrorToString(void)
{
  SDL_dword errorCode = SDLW_GetError();
  if (errorCode == 0)
    return "SDLW_E_OK";

  switch (errorCode)
  {
    case SDLW_E_UNKNOWN_ERROR:     return "An unknown error has occurred."; break;
    case SDLW_E_NOT_INIT:          return "The SDLW_Widgets library has not been initialized."; break;
    case SDLW_E_NOT_IMPLEMENTED:   return "Function not implemented!"; break;
    case SDLW_E_INVALID_PARAMETER: return "Invalid function parameter."; break;
    case SDLW_E_CMDLINE_ERROR:     return "Command line retrieval error."; break;
    case SDLW_E_MEMORY_ERROR:      return "Memory allocation error."; break;
    case SDLW_E_INVALID_CLASS:     return "Invalid widget class."; break;
    case SDLW_E_INVALID_WIDGET:    return "Invalid widget."; break;
    case SDLW_E_INVALID_DATA:      return "Invalid data"; break;
    case SDLW_E_TABLE_EMPTY:       return "Data table empty."; break;
    case SDLW_E_TABLE_FULL:        return "Data table full."; break;
    case SDLW_E_ALREADY_EXISTS:    return "Already exists!"; break;
  }
  return NULL;
}



SDLW_Widget* test_SDLW_CreateWidget(SDLW_WidgetInfo* widgetInfo)
{
  if (widgetInfo == NULL)
    return NULL;

  printf("widgetInfo->title       == \"%s\"\n", widgetInfo->title);
  printf("widgetInfo->Class       == %lu\n", widgetInfo->Class);
  printf("widgetInfo->styles      == %lu\n", widgetInfo->styles);
  printf("widgetInfo->x           == %d\twidgetInfo->y      == %d\n", widgetInfo->x, widgetInfo->y);
  printf("widgetInfo->width       == %d\twidgetInfo->height == %d\n", widgetInfo->width, widgetInfo->height);
  printf("widgetInfo->Parent      == ");

  if (widgetInfo->parent == NULL)
    printf("[0] NULL\n");
  else
  {
    SDLW_WidgetInfo parentInfo;
    SDLW_WidgetID parentID = SDLW_GetWidgetInfo(widgetInfo->parent, &parentInfo);
    if (parentID > 0)
      printf("[%d] \"%s\" (%p)\n", parentID, parentInfo.title, widgetInfo->parent);
    else if (SDLW_GetError() == 0)
      printf("NULL\n");
    else
      printf("NULL; <ERROR:[%lu] %s>\n", SDLW_GetError(), test_SDLW_ErrorToString());
  }

  printf("SDLW_CreateWidget(SDLW_WidgetInfo* widgetInfo) = ");
  SDLW_Widget* result = SDLW_CreateWidget(widgetInfo);
  if (result != NULL)
  {
    SDLW_WidgetID widgetID = SDLW_WidgetToID(result);
    if (widgetID > 0)
      printf("[%d] %p\n", widgetID, result);
    else
      printf("[0] %p; <ERROR: SDLW_WidgetToID failed to obtain ID.\n", result);
  }
  else
    printf("NULL; <ERROR:[%lu] %s>\n", SDLW_GetError(), test_SDLW_ErrorToString());

  printf("\n");
  return result;
}

bool test_SDLW_DestroyWidget(SDLW_Widget* widget)
{
  if (widget == NULL)
    return false;

  SDLW_WidgetInfo widgetInfo;
  SDLW_WidgetID   widgetID = SDLW_GetWidgetInfo(widget, &widgetInfo);
  if (widgetID > 0)
    printf("[%d] SDLW_DestroyWidget(\"%s\") == ", widgetID, widgetInfo.title);
  else
    printf("SDLW_DestroyWidget == ");

  bool result = SDLW_DestroyWidget(widget);
  if (result == true)
    printf("TRUE\n");
  else
    printf("FALSE <ERROR:[0] %s>\n", test_SDLW_ErrorToString());
  return result;
}

SDLW_Widget* test_SDLW_SetParent(SDLW_Widget* widget, SDLW_Widget* newParent)
{
  printf("SDLW_SetParent");
  if (SDLW_CountWidgets() == 0)
  {
    printf("(<widget>, <newParent>) == NULL; <NO WIDGETS>\n");
    return NULL;
  }

  SDLW_WidgetInfo widgetInfo;
  SDLW_WidgetID   widgetID = SDLW_GetWidgetInfo(widget, &widgetInfo);
  if (widgetID == 0)
  {
    printf("(<widget>, <newParent>) == NULL; SDL_GetWidgetInfo failed to obtain <widget> info\n");;
    return NULL;
  }

  printf("(<widget> = [%d] \"%s\", <newParent> = ", widgetID, widgetInfo.title);
  if (newParent == NULL)
    printf("[0] \"<NULL>\")\n");
  else
  {
    SDLW_WidgetInfo newParentInfo;
    SDLW_WidgetID   newParentID = SDLW_GetWidgetInfo(newParent, &newParentInfo);\
    if (newParentID == 0)
      printf("[0], \"<NULL>\"); SDLW_GetWidgetInfo failed to obtain <newParent> info\n");
    else
      printf("[%d] \"%s\")\n", newParentID, newParentInfo.title);
  }

  SDLW_Widget* oldParent = SDLW_SetParent(widget, newParent);
  if (oldParent == NULL && SDLW_GetError() > 0)
    printf("\t<ERROR: SDLW_SetParent failed! %s\n", test_SDLW_ErrorToString());
  return oldParent;
}


bool test_SDLW_EnumWidgets(SDLW_WidgetInfo* widgetInfo)
{
  printf("[SDLW_EnumWidgets(SDLW_WidgetInfo* widgetInfo)\n");
  printf("  widgetInfo->id          == %d\n", SDLW_WidgetToID(widgetInfo->Widget));
  printf("  widgetInfo->title       == \"%s\"\n", widgetInfo->title);
  printf("  widgetInfo->Class       == %lu\n", widgetInfo->Class);
  printf("  widgetInfo->styles      == %lu\n", widgetInfo->styles);
  printf("  widgetInfo->x           == %d\twidgetInfo->y      == %d\n", widgetInfo->x, widgetInfo->y);
  printf("  widgetInfo->width       == %d\twidgetInfo->height == %d\n", widgetInfo->width, widgetInfo->height);
  printf("  widgetInfo->Parent      == ");

  if (widgetInfo->parent != NULL)
  {
    SDLW_WidgetInfo parentInfo;
    SDLW_WidgetID parentID = SDLW_GetWidgetInfo(widgetInfo->parent, &parentInfo);
    if (parentID > 0)
      printf("[%d] \"%s\" (%p)\n", parentID, parentInfo.title, widgetInfo->parent);
    else if (SDLW_GetError() == 0)
      printf("NULL\n");
    else
      printf("NULL; <ERROR:[%lu] %s>", SDLW_GetError(), test_SDLW_ErrorToString());
  }
  else
    printf("NULL\n");

  printf("  widgetInfo->Widget      == %p\n\n", widgetInfo->Widget);
  return true;
}



int testMain_SDLW_Widgets(int argc, char** argv, SDL_Instance instance)
{
  printf("[Tests: argc/argv\n");
  if (SDLW_GetError() == SDLW_E_CMDLINE_ERROR)
    printf("<ERROR>\n\n");
  else
  {
    printf("argc: %d\n", argc);
    for (int i = 0; i < argc; i++)
      printf("argv[%d]: \"%s\"\n", i, argv[i]);
  }
  SDL_Pause;

  SDLW SDLW_Test;
  SDLW_Test.AppName = "testMain_SDLW_Widgets";
 // SDLW_Test.DeviceContext.Type = SDLW_DC_Surface;
  SDLW_Init(&SDLW_Test);


  SDLW_WidgetInfo test_SDLW_WidgetInfo;

  test_SDLW_WidgetInfo.Class     = 0;
  test_SDLW_WidgetInfo.title     = "Star Trek";
  test_SDLW_WidgetInfo.styles    = 10;
  test_SDLW_WidgetInfo.x         = 0;   test_SDLW_WidgetInfo.y      = 0;
  test_SDLW_WidgetInfo.width     = 320; test_SDLW_WidgetInfo.height = 200;
  test_SDLW_WidgetInfo.parent    = NULL;

  SDLW_Widget* test_SDLW_Widget_1 = test_SDLW_CreateWidget(&test_SDLW_WidgetInfo);
  if (test_SDLW_Widget_1 == NULL)
    goto test_SDLW_quit;

  test_SDLW_WidgetInfo.Class     = 2;
  test_SDLW_WidgetInfo.title     = "Lord of the Rings";
  test_SDLW_WidgetInfo.styles    = 250;
  test_SDLW_WidgetInfo.x         = 640;  test_SDLW_WidgetInfo.y      = 480;
  test_SDLW_WidgetInfo.width     = 1024; test_SDLW_WidgetInfo.height = 720;
  test_SDLW_WidgetInfo.parent    = test_SDLW_Widget_1;

  SDLW_Widget* test_SDLW_Widget_2 = test_SDLW_CreateWidget(&test_SDLW_WidgetInfo);
  if (test_SDLW_Widget_2 == NULL)
    goto test_SDLW_quit;

  test_SDLW_WidgetInfo.Class     = 50;
  test_SDLW_WidgetInfo.title     = "Harry Potter";
  test_SDLW_WidgetInfo.styles    = 250;
  test_SDLW_WidgetInfo.x         = 640;  test_SDLW_WidgetInfo.y      = 480;
  test_SDLW_WidgetInfo.width     = 1024; test_SDLW_WidgetInfo.height = 720;
  test_SDLW_WidgetInfo.parent    = NULL;

  SDLW_Widget* test_SDLW_Widget_3 = test_SDLW_CreateWidget(&test_SDLW_WidgetInfo);
  if (test_SDLW_Widget_2 == NULL)
    goto test_SDLW_quit;
  SDLW_EnumWidgets(test_SDLW_EnumWidgets);

  test_SDLW_SetParent(test_SDLW_Widget_3, test_SDLW_Widget_2);
  SDLW_EnumWidgets(test_SDLW_EnumWidgets);


  test_SDLW_DestroyWidget(test_SDLW_Widget_3);
  test_SDLW_DestroyWidget(test_SDLW_Widget_1);
  SDLW_EnumWidgets(test_SDLW_EnumWidgets);

test_SDLW_quit:
  //SDLW_Quit();
  //printf("[SDLW_DestroyAllWidgets]\n");
  //SDLW_EnumWidgets(test_SDLW_EnumWidgets);
  SDL_Pause;
  return 0;
}
declare_SDLW_main(testMain_SDLW_Widgets);
