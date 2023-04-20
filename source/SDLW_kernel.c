#include <SDL_Widgets/SDLW_kernel.h>
#include <SDL_Widgets/SDLW_error.h>
#include "SDLW_internals.h"

int SDLW_CallMainProc
 (SDLW_fnMain fnMain, int argc, char** argv, SDL_Instance instance)
{
  SDLW_ClearError();
  if (fnMain == NULL)
  {
    SDLW_SetError(SDLW_E_NOT_IMPLEMENTED);
    return -1;
  }

 #ifdef WINDOWS
  argv = CommandLineToArgvA(GetCommandLineA(), &argc);
  if (argv == NULL)
    SDLW_SetError(SDLW_E_CMDLINE_ERROR);
 #endif

  int result = (fnMain)(argc, argv, instance);
 #ifdef WINDOWS
  LocalFree(argv);
 #endif
  return result;
}





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
      m_SDLW.fWasInit = true;
      m_SDLW.Info.AppName = info->AppName;
     #ifdef WINDOWS
      if (info->Instance != NULL)
        m_SDLW.Info.Instance = info->Instance;
      else
        m_SDLW.Info.Instance = (HINSTANCE) GetModuleHandle(NULL);
     #elif defined(LINUX)
      pid_t pid = fork();
      if (pid == 0)
        m_SDLW.Info.Instance = getpid();
      else
        m_SDLW.Info.Instance = getppid();
     #endif // WINDOWS
     // m_SDLW.Info.DeviceContext.Type = info->DeviceContext.Type;
     // m_SDLW.fWasInit = true;
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

void SDLW_DestroyWidgetHelper
 (struct SDLW_Widget** widgetTable,
  struct SDLW_Widget* widget)
{
  if (*widgetTable == widget)
    *widgetTable = widget->pNext;

  if (widget->pNext != NULL)
    widget->pNext->pPrev = widget->pPrev;
  free(widget); widget = NULL;
}

bool SDLW_Quit(void)
{
  SDLW_ClearError();
  if (m_SDLW.fWasInit == false)
  {
    SDLW_SetError(SDLW_E_NOT_INIT);
    return false;
  }

  if (m_SDLW.widgets > 0)
  {
    struct SDLW_Widget* widget = m_SDLW.widgetTable;
    while (widget != NULL)
    {
      SDLW_DestroyWidget(widget);
      widget = widget->pNext;
    }
  }

  memset(&m_SDLW, 0, sizeof(SDLW));
  m_SDLW.fWasInit = false;
  SDL_Quit();
  return true;
}



int SDLW_CLI_Parse(int argc, char** argv, int (fnCLI_Parser)(int argi, SDL_string arg, SDL_string argn))
{
  SDLW_ClearError();
  if (argv == NULL || fnCLI_Parser == NULL)
  {
    SDLW_SetError(SDLW_E_NOT_IMPLEMENTED);
    return -1;
  }

 #ifdef WINDOWS
  argv = CommandLineToArgvA(GetCommandLineA(), &argc);
 #endif

  for (int i = 0; i < argc; i++)
  {
    int r = (fnCLI_Parser)(i, argv[i], argv[i+1]);
    if (r != 0)
      return r;
  }
  return -2;
}

int SDLW_CLI_ParseW(int argc, wchar_t** argv, int (fnCLI_Parser)(int argi, SDL_Wstring arg, SDL_Wstring argn))
{
  SDLW_ClearError();
  if (argv == NULL || fnCLI_Parser == NULL)
  {
    SDLW_SetError(SDLW_E_NOT_IMPLEMENTED);
    return -1;
  }

 #ifdef WINDOWS
  argv = CommandLineToArgvW(GetCommandLineW(), &argc);
 #endif

  for (int i = 0; i < argc; i++)
  {
    int r = (fnCLI_Parser)(i, argv[i], argv[i+1]);
    if (r != 0)
      return r;
  }
  return -2;
}






SDLW_Widget* SDLW_CreateWidget(SDLW_WidgetInfo* widgetInfo)
{
  SDLW_ClearError();
  if (m_SDLW.fWasInit == false)
  {
    SDLW_SetError(SDLW_E_NOT_INIT);
    return false;
  }

  if (m_SDLW.widgets > 65535)
  {
    SDLW_SetError(SDLW_E_TABLE_FULL);
    return false;
  }

  if (widgetInfo == NULL || (widgetInfo->Class == 1 && widgetInfo->title == NULL))
  {
    SDLW_SetError(SDLW_E_INVALID_PARAMETER);
    return NULL;
  }

  struct SDLW_Widget* newWidget = calloc(1, sizeof(struct SDLW_Widget));
  if (newWidget == NULL)
    return NULL;

  m_SDLW.widgets++;
  newWidget->Class  = widgetInfo->Class;
  newWidget->title  = widgetInfo->title;
  newWidget->id     = m_SDLW.widgets;
  newWidget->styles = widgetInfo->styles;
  newWidget->x      = widgetInfo->x;     newWidget->y      = widgetInfo->y;
  newWidget->width  = widgetInfo->width; newWidget->height = widgetInfo->height;
  newWidget->data = widgetInfo->data;

  if (m_SDLW.widgetTable == NULL)
  {
    newWidget->pNext = newWidget->pPrev = newWidget;
    m_SDLW.widgetTable = newWidget;
    return newWidget;
  }

  struct SDLW_Widget* lastWidget = m_SDLW.widgetTable->pPrev;
  newWidget->pNext = m_SDLW.widgetTable;
  m_SDLW.widgetTable->pPrev = newWidget;
  newWidget->pPrev = lastWidget;
  lastWidget->pNext = newWidget;

  if (m_SDLW.widgets == 1)
    newWidget->parent = NULL;
  else
    SDLW_SetParent(newWidget, widgetInfo->parent);

  return newWidget;
}

bool SDLW_DestroyWidget(SDLW_Widget* widget)
{
  SDLW_ClearError();
  if (m_SDLW.widgets == 0)
  {
    SDLW_SetError(SDLW_E_TABLE_EMPTY);
    return false;
  }

  if (widget == NULL)
  {
    SDLW_SetError(SDLW_E_INVALID_PARAMETER);
    return false;
  }

  struct SDLW_Widget* pWidget = m_SDLW.widgetTable, *lastWidget = NULL;
  while (pWidget->id != widget->id)
  {
    if (pWidget->pNext == m_SDLW.widgetTable)
    {
      SDLW_SetError(SDLW_E_INVALID_WIDGET);
      return false;
    }

    lastWidget = pWidget;
    pWidget = pWidget->pNext;
  }

  struct SDLW_Widget* pParentWidget = m_SDLW.widgetTable;
  while (pParentWidget->pNext != m_SDLW.widgetTable)
  {
    if (pParentWidget->parent != NULL &&
        pParentWidget->parent->id == widget->id)
      pParentWidget->parent = NULL;
    pParentWidget = pParentWidget->pNext;
  }

 // Because our widget data is stored within a Circular Doubly Linked List (CDL),
 // the following code is required to properly find enumerate
 // identifier (ID):
  if (pParentWidget->parent != NULL &&
      pParentWidget->parent->id == widget->id)
    pParentWidget->parent = NULL;

  if (pWidget->pNext == m_SDLW.widgetTable && lastWidget == NULL)
    m_SDLW.widgetTable = NULL;
  else if (pWidget == m_SDLW.widgetTable)
  {
    lastWidget = m_SDLW.widgetTable->pPrev;
    m_SDLW.widgetTable = m_SDLW.widgetTable->pNext;
    lastWidget->pNext = m_SDLW.widgetTable;
    m_SDLW.widgetTable->pPrev = lastWidget;
  }
  else if (pWidget->pNext == m_SDLW.widgetTable)
  {
    lastWidget->pNext = m_SDLW.widgetTable;
    m_SDLW.widgetTable = lastWidget;
  }
  else
  {
    struct SDLW_Widget* temp = pWidget->pNext;
    lastWidget->pNext = temp;
    temp->pPrev = lastWidget;
  }
  free(pWidget); m_SDLW.widgets--;
  return true;
}


SDLW_WidgetID SDLWAPI SDLW_CountWidgets(void)
{
  return m_SDLW.widgets;
}

SDLW_WidgetID SDLW_GetWidgetInfo(SDLW_Widget* widget, SDLW_WidgetInfo* widgetInfo)
{
  SDLW_ClearError();
  if (m_SDLW.fWasInit == false)
  {
    SDLW_SetError(SDLW_E_NOT_INIT);
    return 0;
  }

  if (widgetInfo == NULL)
  {
    SDLW_SetError(SDLW_E_INVALID_PARAMETER);
    return 0;
  }

  struct SDLW_Widget* pWidget = m_SDLW.widgetTable;
  while (pWidget->pNext != m_SDLW.widgetTable)
  {
    if (pWidget->id == widget->id)
    {
      widgetInfo->Class  = pWidget->Class;
      widgetInfo->title  = pWidget->title;
      widgetInfo->id     = pWidget->id;
      widgetInfo->styles = pWidget->styles;
      widgetInfo->x      = pWidget->x;     widgetInfo->y      = widget->y;
      widgetInfo->width  = pWidget->width; widgetInfo->height = widget->height;
      widgetInfo->parent = (m_SDLW.widgets > 1 ? widgetInfo->parent : NULL);
      return pWidget->id;
    }
    pWidget = pWidget->pNext;
  }

 // Since our widget data is stored within a Circular Doubly Linked List (CDL),
 // the following code is required to properly find our desired widget in the
 // head:
  if (pWidget->id == widget->id)
  {
    widgetInfo->Widget = pWidget;
    widgetInfo->Class  = pWidget->Class;
    widgetInfo->title  = pWidget->title;
    widgetInfo->id     = pWidget->id;
    widgetInfo->styles = pWidget->styles;
    widgetInfo->x      = pWidget->x;     widgetInfo->y      = widget->y;
    widgetInfo->width  = pWidget->width; widgetInfo->height = widget->height;
    widgetInfo->parent = (m_SDLW.widgets > 1 ? widgetInfo->parent : NULL);
    widgetInfo->data = pWidget->data;
    return pWidget->id;
  }
  SDLW_SetError(SDLW_E_INVALID_WIDGET);
  return 0;
}

SDLW_Widget* SDLW_WidgetFromID(SDLW_WidgetID widgetID)
{
  SDLW_ClearError();
  if (m_SDLW.fWasInit == false)
  {
    SDLW_SetError(SDLW_E_NOT_INIT);
    return 0;
  }

  if (m_SDLW.widgets == 0)
  {
    SDLW_SetError(SDLW_E_TABLE_EMPTY);
    return NULL;
  }

  struct SDLW_Widget* pWidget = m_SDLW.widgetTable;
  while (pWidget->pNext != m_SDLW.widgetTable)
  {
    if (pWidget->id == widgetID)
      return pWidget;
    pWidget = pWidget->pNext;
  }

 // Since our widget data is stored within a Circular Doubly Linked List (CDL),
 // the following code is required to properly find our desired widget in the
 // head:
  if (pWidget->id == widgetID)
    return pWidget;
  SDLW_SetError(SDLW_E_INVALID_WIDGET);
  return NULL;
}

SDLW_WidgetID SDLW_WidgetToID(SDLW_Widget* widget)
{
  SDLW_ClearError();
  if (m_SDLW.fWasInit == false)
  {
    SDLW_SetError(SDLW_E_NOT_INIT);
    return 0;
  }

  if (m_SDLW.widgets == 0)
  {
    SDLW_SetError(SDLW_E_TABLE_EMPTY);
    return 0;
  }

  struct SDLW_Widget* pWidget = m_SDLW.widgetTable;
  while (pWidget->pNext != m_SDLW.widgetTable)
  {
    if (pWidget->id == widget->id)
      return pWidget->id;
    pWidget = pWidget->pNext;
  }

 // Since our widget data is stored within a Circular Doubly Linked List (CDL),
 // the following code is required to properly find our desired widget in the
 // head:
  if (pWidget->id == widget->id)
    return pWidget->id;
  SDLW_SetError(SDLW_E_INVALID_WIDGET);
  return 0;
}

SDLW_Widget* SDLW_SetParent(SDLW_Widget* widget, SDLW_Widget* newParent)
{
  SDLW_ClearError();
  if (m_SDLW.fWasInit == false)
  {
    SDLW_SetError(SDLW_E_NOT_INIT);
    return NULL;
  }

  if (widget == NULL)
  {
    SDLW_SetError(SDLW_E_INVALID_PARAMETER);
    return 0;
  }

  if (m_SDLW.widgets == 1)
  {
    SDLW_SetError(SDLW_E_TABLE_FULL);
    widget->parent = NULL;
    return widget;
  }

  struct SDLW_Widget* pWidget = m_SDLW.widgetTable;
  while (pWidget->pNext != m_SDLW.widgetTable)
  {
    if (pWidget->id == widget->id)
    {
      struct SDLW_Widget* oldParent = pWidget->parent;
      pWidget->parent = newParent;
      return oldParent;
    }
    pWidget = pWidget->pNext;
  }

 // Because our widget data is stored within a Circular Doubly Linked List (CDL),
 // the following code piece is required to properly find our desired widget
 // identifier (ID):
  if (pWidget->id == widget->id)
  {
    struct SDLW_Widget* oldParent = pWidget;
    pWidget->parent = newParent;
    return oldParent;
  }
  SDLW_SetError(SDLW_E_INVALID_WIDGET);
  return NULL;
}

SDLW_Widget* SDLW_GetParent(SDLW_Widget* widget)
{
  SDLW_WidgetInfo widgetInfo;
  return (SDLW_GetWidgetInfo(widget, &widgetInfo) > 0 ? widgetInfo.parent : 0);
}


bool SDLW_EnumWidgets(SDLW_WidgetEnumProc lpfnWidgetEnum)
{
  SDLW_ClearError();
  if (m_SDLW.fWasInit == false)
  {
    SDLW_SetError(SDLW_E_NOT_INIT);
    return false;
  }

  if (m_SDLW.widgets == 0)
  {
    SDLW_SetError(SDLW_E_TABLE_EMPTY);
    return false;
  }

  if (lpfnWidgetEnum == NULL)
  {
    SDLW_SetError(SDLW_E_INVALID_PARAMETER);
    return false;
  }

  SDLW_Widget* widget = m_SDLW.widgetTable;
  while (widget->pNext != m_SDLW.widgetTable)
  {
    SDLW_WidgetInfo widgetInfo;
    widgetInfo.Widget = widget;
    widgetInfo.Class  = widget->Class;
    widgetInfo.title  = widget->title;
    widgetInfo.id     = widget->id;
    widgetInfo.styles = widget->styles;
    widgetInfo.x      = widget->x;     widgetInfo.y      = widget->y;
    widgetInfo.width  = widget->width; widgetInfo.height = widget->height;
    widgetInfo.parent = (m_SDLW.widgets > 1 ? widget->parent : NULL);
    widgetInfo.data   = widget->data;

    bool result = (lpfnWidgetEnum)(&widgetInfo);
    if (result == false)
      return false;
    widget = widget->pNext;
  }

 // Because our widget data is stored within a Circular Doubly Linked List (CDL),
 // the following code is required to properly find enumerate
 // identifier (ID):
  SDLW_WidgetInfo widgetInfo;
  widgetInfo.Widget = widget;
  widgetInfo.Class  = widget->Class;
  widgetInfo.title  = widget->title;
  widgetInfo.id     = widget->id;
  widgetInfo.styles = widget->styles;
  widgetInfo.x      = widget->x;     widgetInfo.y      = widget->y;
  widgetInfo.width  = widget->width; widgetInfo.height = widget->height;
  widgetInfo.parent = (m_SDLW.widgets > 1 ? widget->parent : NULL);
  widgetInfo.data   = widget->data;

  bool result = (lpfnWidgetEnum)(&widgetInfo);
  if (result == false)
    return false;

  return true;
}





#ifdef WINDOWS
// CommandLineArgvA is borrowed code from ReactOS at:
// https://github.com/reactos/reactos/blob/master/dll/win32/shell32/wine/shell32_main.c
// and modified for (A)scii equivalent of (W)ide character acceptance.
LPSTR* SDLWAPI CommandLineToArgvA(LPCSTR lpCmdline, int* numargs)
{
  DWORD argc;
  LPSTR  *argv;
  LPCSTR s;
  LPSTR d;
  LPSTR cmdline;
  int qcount,bcount;

  if(!numargs)
  {
    SDLW_SetError(ERROR_INVALID_PARAMETER);
    return NULL;
  }

  if (*lpCmdline==0)
  {
   /* Return the path to the executable */
    DWORD len, deslen=MAX_PATH, size;

    size = sizeof(LPSTR)*2 + deslen*sizeof(char);
    for (;;)
    {
      if (!(argv = (LPSTR*) LocalAlloc(LMEM_FIXED, size))) return NULL;
      len = GetModuleFileNameA(0, (LPSTR)(argv+2), deslen);
      if (!len)
      {
          LocalFree(argv);
          return NULL;
      }
      if (len < deslen)
        break;
      deslen*=2;
      size = sizeof(LPSTR)*2 + deslen*sizeof(char);
      LocalFree( argv );
    }
    argv[0]=(LPSTR)(argv+2);
    argv[1]=NULL;
    *numargs=1;

    return argv;
  }

 /* --- First count the arguments */
  argc=1;
  s=lpCmdline;
 /* The first argument, the executable path, follows special rules */
  if (*s=='"')
  {
   /* The executable path ends at the next quote, no matter what */
    s++;
    while (*s)
      if (*s++=='"')
        break;
  }
  else
  {
   /* The executable path ends at the next space, no matter what */
    while (*s && *s!=' ' && *s!='\t')
      s++;
  }
  /* skip to the first argument, if any */
  while (*s==' ' || *s=='\t')
    s++;
  if (*s)
    argc++;

 /* Analyze the remaining arguments */
  qcount=bcount=0;
  while (*s)
  {
    if ((*s==' ' || *s=='\t') && qcount==0)
    {
     /* skip to the next argument and count it if any */
      while (*s==' ' || *s=='\t')
        s++;
      if (*s)
        argc++;
      bcount=0;
    }
    else if (*s=='\\')
    {
     /* '\', count them */
      bcount++;
      s++;
    }
    else if (*s=='"')
    {
     /* '"' */
      if ((bcount & 1)==0)
        qcount++; /* unescaped '"' */
      s++;
      bcount=0;
     /* consecutive quotes, see comment in copying code below */
      while (*s=='"')
      {
        qcount++;
        s++;
      }
      qcount=qcount % 3;
      if (qcount==2)
        qcount=0;
    }
    else
    {
     /* a regular character */
      bcount=0;
      s++;
    }
  }

 /* Allocate in a single lump, the string array, and the strings that go
  * with it. This way the caller can make a single LocalFree() call to free
  * both, as per MSDN.
  */
  argv= (LPSTR*) LocalAlloc(LMEM_FIXED, (argc+1)*sizeof(LPSTR)+(strlen(lpCmdline)+1)*sizeof(char));
  if (!argv)
    return NULL;
  cmdline=(LPSTR)(argv+argc+1);
  strcpy(cmdline, lpCmdline);

 /* --- Then split and copy the arguments */
  argv[0]=d=cmdline;
  argc=1;
 /* The first argument, the executable path, follows special rules */
  if (*d=='"')
  {
   /* The executable path ends at the next quote, no matter what */
    s=d+1;
    while (*s)
    {
      if (*s=='"')
      {
        s++;
        break;
      }
      *d++=*s++;
    }
  }
  else
  {
   /* The executable path ends at the next space, no matter what */
    while (*d && *d!=' ' && *d!='\t')
      d++;
    s=d;
    if (*s)
     s++;
  }
 /* close the executable path */
  *d++=0;
 /* skip to the first argument and initialize it if any */
  while (*s==' ' || *s=='\t')
   s++;
  if (!*s)
  {
   /* There are no parameters so we are all done */
    argv[argc]=NULL;
    *numargs=argc;
    return argv;
  }

 /* Split and copy the remaining arguments */
  argv[argc++]=d;
  qcount=bcount=0;
  while (*s)
  {
    if ((*s==' ' || *s=='\t') && qcount==0)
    {
        /* close the argument */
        *d++=0;
        bcount=0;

        /* skip to the next one and initialize it if any */
        do {
            s++;
        } while (*s==' ' || *s=='\t');
        if (*s)
            argv[argc++]=d;
    }
    else if (*s=='\\')
    {
        *d++=*s++;
        bcount++;
    }
    else if (*s=='"')
    {
      if ((bcount & 1)==0)
      {
       /* Preceded by an even number of '\', this is half that
        * number of '\', plus a quote which we erase.
        */
        d-=bcount/2;
        qcount++;
      }
      else
      {
       /* Preceded by an odd number of '\', this is half that
        * number of '\' followed by a '"'
        */
        d=d-bcount/2-1;
        *d++='"';
      }
      s++;
      bcount=0;
     /* Now count the number of consecutive quotes. Note that qcount
      * already takes into account the opening quote if any, as well as
      * the quote that lead us here.
      */
      while (*s=='"')
      {
        if (++qcount==3)
        {
          *d++='"';
          qcount=0;
        }
        s++;
      }
      if (qcount==2)
        qcount=0;
    }
    else
    {
     /* a regular character */
      *d++=*s++;
      bcount=0;
    }
  }
  *d='\0';
  argv[argc]=NULL;
  *numargs=argc;

  return argv;
}
#endif // WINDOWS
