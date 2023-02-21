#ifdef WINDOWS
#include <windows.h>
#endif
#include <SDL_Widgets.h>

bool SDLW_TestClassProc1(SDLW_Event* event);
bool SDLW_TestClassProc2(SDLW_Event* event);

#ifdef WINDOWS
int WinMain(HINSTANCE hInstance, HINSTANCE hInstPrev, LPSTR pszCmdLine, int nShowCmd)
#else
int main(int argc, char** argv)
#endif // WINDOWS
{
  SDLW SDLW_Test;
  SDLW_Test.AppName = "SDLW_TestClass";
  SDLW_Test.DeviceContext.Type = SDLW_DC_Surface;
  SDLW_Init(&SDLW_Test);


  SDLW_Class SDLW_TestClass1;
  SDLW_TestClass1.className = "SDLW_TestClass.1";
  SDLW_TestClass1.procedure = SDLW_TestClassProc1;

  SDL_byte classTest1 = SDLW_RegisterClass(&SDLW_TestClass1);
  printf("SDLW_RegisterClass(&SDLW_TestClass1) returned %d\n", classTest1);
  if (SDLW_GetError() > 0)
  {
    printf("SDLW_GetError() returned %d\n", SDLW_GetError());
    goto SDLW_TestQuit;
  }

  SDLW_Class SDLW_TestClass2;
  SDLW_TestClass2.className = "SDLW_TestClass.2";
  SDLW_TestClass2.procedure = SDLW_TestClassProc2;

  SDL_byte classTest2 = SDLW_RegisterClass(&SDLW_TestClass2);
  printf("SDLW_RegisterClass(&SDLW_TestClass2) returned %d\n", classTest2);
  if (SDLW_GetError() > 0)
  {
    printf("SDLW_GetError() returned %d\n", SDLW_GetError());
    goto SDLW_TestQuit;
  }


  SDLW_WidgetInfo widgetInfo1;
  widgetInfo1.className = SDLW_TestClass1.className;
  widgetInfo1.styles    = 10;
  widgetInfo1.x     = 0; widgetInfo1.y        = 0;
  widgetInfo1.width = 320; widgetInfo1.height = 200;

  SDLW_Widget* widget1 = SDLW_CreateWidget(&widgetInfo1);
  if (widget1 == NULL)
  {
    printf("SDLW_CreateWidget(widget1) returned NULL (Error Code: %d\n", SDLW_GetError());
    goto SDLW_TestQuit;
  }


  SDLW_WidgetInfo widgetInfo2;
  widgetInfo2.className = SDLW_TestClass2.className;
  widgetInfo2.styles    = 250;
  widgetInfo2.x     = 600;  widgetInfo2.y      = 800;
  widgetInfo2.width = 1024; widgetInfo2.height = 900;
  widgetInfo2.parent = widget1;

  SDLW_Widget* widget2 = SDLW_CreateWidget(&widgetInfo2);
  if (widget2 == NULL)
  {
    printf("SDLW_CreateWidget(widget2) returned NULL (Error Code: %d\n", SDLW_GetError());
    goto SDLW_TestQuit;
  }

  printf("\n");

  SDLW_CallWidgetProc(widget1);
  SDLW_CallWidgetProc(widget2);


SDLW_TestQuit:
  SDLW_Quit();
  SDL_Pause;
  return 0;
}

bool SDLW_TestClassProc1(SDLW_Event* event)
{
  SDLW_WidgetInfo widgetInfo;
  SDLW_GetWidgetInfo(event->widget, &widgetInfo);
  printf("[Widget Info: %d]\n", widgetInfo.id);
  printf("Class: \"%s\"\n", widgetInfo.className);
  printf("Styles: %d\n", widgetInfo.styles);
  printf("X: %d\n", widgetInfo.x);
  printf("Y: %d\n", widgetInfo.y);
  printf("Width: %d\n", widgetInfo.width);
  printf("Height: %d\n", widgetInfo.height);
  if (widgetInfo.parent == NULL)
    printf("Parent: NULL\n");
  else
    printf("Parent: \"%s\"\n", SDLW_GetWidgetClass(widgetInfo.parent));
  printf("\n");
  return true;
}

bool SDLW_TestClassProc2(SDLW_Event* event)
{
  SDLW_WidgetInfo widgetInfo;
  SDLW_GetWidgetInfo(event->widget, &widgetInfo);
  printf("[Widget Info: %d]\n", widgetInfo.id);
  printf("Class: \"%s\"\n", widgetInfo.className);
  printf("Styles: %d\n", widgetInfo.styles);
  printf("X: %d\n", widgetInfo.x);
  printf("Y: %d\n", widgetInfo.y);
  printf("Width: %d\n", widgetInfo.width);
  printf("Height: %d\n", widgetInfo.height);
  if (widgetInfo.parent == NULL)
    printf("Parent: NULL\n");
  else
    printf("Parent: \"%s\"\n", SDLW_GetWidgetClass(widgetInfo.parent));
  printf("\n");
  return true;
}
