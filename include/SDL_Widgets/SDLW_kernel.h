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
 *  \file SDLW_kernel.h
 *
 *  Kernel header for the SDL_Widgets library.
 **/

#ifndef SDLW_kernel_h_
#define SDLW_kernel_h_

#include "SDLW_defines.h"
//#include "SDLW_video.h"

#ifdef WINDOWS
typedef HINSTANCE SDL_Instance;
#elif defined(LINUX)
typedef pid_t SDL_Instance;
#else
typedef int SDL_Instance;
#endif

/**
 * User-defined entry point for SDL_Widgets
 *
 * \param fnMain Entry-point placeholder procedure
 * \param argc Command line argument counter
 * \param argv Command lint argument strings array
 * \param instance Application instance
 *
 * Returns 0 on success; negative error code on failure.
 * Call SDLW_GetError for more error-related information.
 *
 * \usage Define a named <entry> function as:
 *        int <entry>(argc, char** argv, SDL_Instance instance)
 *        {
 *          <...!!!CODE GOES HERE!!!...>
 *          return 0;
 *        }
 *        declare_SDLW_main(<entry>)
 *
 * Whereas <entry> is your user-defined function.
 * Depending upon the underlying OS, declare_SDLW_main
 * sets up the real entry point 'WinMain' or 'main'.
 *
 * \ss SDLW_CallMainProc
 **/
typedef int (*SDLW_fnMain)(int argc, char** argv, SDL_Instance instance);
int SDLWAPI SDLW_CallMainProc(SDLW_fnMain fnMain, int argc, char** argv, SDL_Instance instance);

 #ifndef BUILDLIB
 #ifdef WINDOWS
  #define declare_SDLW_main(m) \
   int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInst, LPSTR lpCmdLine, int nShowCmd) \
   { return SDLW_CallMainProc(m, 0, NULL, hInstance); }
  #else
   #define declare_SDLW_main(m) \
    int main(int argc, char** argv); \
    { return SDLW_CallMainProc(m, argc, argv, 0); }
  #endif
 #endif


typedef struct SDLW
{
    SDL_string          AppName;
    SDL_Instance        Instance;
    int                 nCmdShow;
    Uint32              flags;
   // SDLW_DeviceContext  DeviceContext;
} SDLW;

void       SDLWAPI SDLW_Init(SDLW* info);
bool       SDLWAPI SDLW_WasInit(void);
SDL_string SDLWAPI SDLW_GetAppName(void);
bool       SDLWAPI SDLW_Quit(void);

int SDLWAPI SDLW_CLI_Parse(int argc, char** argv, int (fnCLI_Parser)(int argi, SDL_string arg, SDL_string argn));
int SDLWAPI SDLW_CLI_ParseW(int argc, wchar_t** argv, int (fnCLI_Parser)(int argi, SDL_Wstring arg, SDL_Wstring argn));



typedef Uint32  SDLW_DisplayID;
typedef struct  SDLW_Display SDLW_Display;

SDLW_Display* SDLWAPI SDLW_CreateDisplay(SDL_string title, int x, int y, int h, int w, Uint32 flags);
bool          SDLWAPI SDLW_DestroyDisplay(SDLW_Display* display);


typedef struct     SDLW_Widget  SDLW_Widget;   /**< Widget handle */
typedef SDL_Uint   SDLW_WidgetID;              /**< Widget Identifier (ID) */
typedef SDL_dword  SDLW_ClassID;               /**< Widget Class Identifier (ID) */


// [Widget Class Identifiers (ClassID's)]
//
#define SDLW_WC_Window        0L  /**< Window */
#define SDLW_WC_Dialog        1L  /**< Dialog */
#define SDLW_WC_Button        2L  /**< Button */
#define SDLW_WC_Menu          3L  /**< Menu */
#define SDLW_WC_ToolBar       4L  /**< Toolbar */
#define SDLW_WC_StatusBar     5L  /**< Status Bar */
#define SDLW_WC_ScrollBar     6L  /**< Scroll Bar */
#define SDLW_WC_TabBar        7L  /**< Tab Bar */
#define SDLW_WC_Document      8L  /**< Document */
#define SDLW_WC_ListView      9L  /**< List View */
#define SDLW_WC_TreeView     10L  /**< Tree View */
#define SDLW_WC_ProgressBar  11L  /**< Progress Bar */
#define SDLW_WC_Slider       12L  /**< Slider */
#define SDLW_WC_SplitBar     13L  /**< Split Bar */
#define SDLW_WC_ToolBook     14L  /**< Tool Book */
#define SDLW_WC_Calander     15L  /**< Calender */
#define SDLW_WC_DateTime     16L  /**< Date/Time Picker */
#define SDLW_WC_ColorWheel   17L  /**< Color Picker */
#define SDLW_WC_VideoRes     18L  /**< Video Resolution Picker */

typedef struct SDLW_Event
{
    SDLW_Widget* widget;
    SDL_Uint     message;
    SDL_Uint     item;
    void*        data;
} SDLW_Event;

typedef struct SDLW_WidgetInfo
{
    SDLW_ClassID   Class;
    SDL_string     title;
    SDLW_WidgetID  id;
    SDL_dword      styles;
    int            x,     y;
    int            width, height;
    SDLW_Widget*   parent;
    void*          data;
    SDLW_Widget*   Widget;
} SDLW_WidgetInfo;


/**
 * Creates a widget
 *
 * \param widgetInfo Structure pointer containing widget-creation data
 *
 * Returns non-NULL on success; NULL on failure.
 * Call SDLW_GetError for more error-related information.
 *
 * \usage Creates a <widgetInfo> defined widget.
 *
 * \ss SDLW_CreateWidget
 **/
SDLW_Widget* SDLWAPI SDLW_CreateWidget(SDLW_WidgetInfo* widgetInfo);


/**
 * Destroys a widget
 *
 * \param widget The widget to destroy
 *
 * Returns true on success; false on failure.
 * Call SDLW_GetError for more error-related information.
 *
 * \usage Destroys a widget defined by <widget>.
 *
 * \ss SDLW_DestroyWidget
 **/
bool SDLWAPI SDLW_DestroyWidget(SDLW_Widget* widget);

/**
 * Obtains an internal widget count
 *
 * Returns the total number of created widgets.
 * Call SDLW_GetError for more error-related information.
 *
 * \usage Obtains the total number of created widgets.
 *
 * \ss SDLW_CountWidgets
 **/
SDLW_WidgetID SDLWAPI SDLW_CountWidgets(void);

/**
 * Obtains widget information
 *
 * \param widget Widget handle
 * \param widgetInfo Structure pointer to contain information
 *
 * Returns negative number on success; 0 on failure.
 * Call SDLW_GetError for more error-related information.
 *
 * \usage Obtains widget information
 *
 * \ss SDLW_GetWidgetInfo
 **/
SDLW_WidgetID SDLWAPI SDLW_GetWidgetInfo(SDLW_Widget* widget, SDLW_WidgetInfo* widgetInfo);

/**
 * Converts a widget handle to a widget identifier
 *
 * \param widgetID Widget identifier
 *
 * Returns non-NULL on success; NULL on error
 * Call SDLW_GetError for more error-related information.
 *
 * \usage Converts a widget handle to a widget identifier
 *
 * \ss SDLW_WidgetFromID
 **/
SDLW_Widget* SDLWAPI SDLW_WidgetFromID(SDLW_WidgetID widgetID);

/**
 * Converts a widget identifier to a widget handle
 *
 * \param widget Widget handle
 *
 * Returns positive number on success; 0 = no widgets or error
 * Call SDLW_GetError for more error-related information.
 *
 * \usage Converts a widget identifier to a widget handle
 *
 * \ss SDLW_WidgetToID
 **/
SDLW_WidgetID SDLWAPI SDLW_WidgetToID(SDLW_Widget* widget);


/**
 * Set a widget parent
 *
 * \param widget Widget parent
 *
 * Returns non-NULL on success; NULL on failure
 * Call SDLW_GetError for more error-related information.
 *
 * \usage Set a widget parent
 *
 * \ss SDLW_SetParent
 **/
SDLW_Widget* SDLWAPI SDLW_SetParent(SDLW_Widget* widget, SDLW_Widget* newParent);

/**
 * Get a widget parent
 *
 * \param widget Widget child
 *
 * Returns non-NULL on success; NULL on failure
 * Call SDLW_GetError for more error-related information.
 *
 * \usage Gets a widget parent
 *
 * \ss SDLW_GetParent
 **/
SDLW_Widget* SDLWAPI SDLW_GetParent(SDLW_Widget* widget);


/**
 * Enumerates created widgets
 *
 * \param lpfnWidgetEnum User-defined widget enumeration function
 *
 * Returns non-NULL on success; NULL on failure
 * Call SDLW_GetError for more error-related information.
 *
 * \usage Define a named <lpfnWidgetEnum> function as:
 *        bool <lpfnWidgetEnum>(SDLW_WidgetInfo* widgetInfo)
 *        {
 *          <...!!!CODE GOES HERE!!!...>
 *          return true;
 *        }
 *
 * Whereas <lpfnWidgetEnum> is the name of your user-defined function.
 * Then in your main or WinMain function, call SDLW_EnumWidgets(<lpfnWidgetEnum>).
 *
 * \ss SDLW_WidgetEnumProc
 * \ss SDLW_EnumWidgets
 **/
typedef bool (*SDLW_WidgetEnumProc)(SDLW_WidgetInfo* widgetInfo);
bool SDLWAPI SDLW_EnumWidgets(SDLW_WidgetEnumProc lpfnWidgetEnum);



typedef struct   SDLW_Button      SDLW_Button;
typedef struct   SDLW_Menu        SDLW_Menu;
typedef SDL_Uint SDLW_MenuID;
typedef struct   SDLW_Toolbar     SDLW_Toolbar;
typedef SDL_byte SDLW_ToolID;
typedef struct   SDLW_StatusBar   SDLW_StatusBar;
typedef struct   SDLW_ListView    SDLW_ListView;
typedef struct   SDLW_TreeView    SDLW_TreeView;
typedef struct   SDLW_TabBar      SDLW_TabBar;
typedef SDL_byte SDLW_TabID;
typedef struct   SDLW_ProgressBar SDLW_ProgressBar;
typedef struct   SDLW_Slider      SDLW_Slider;
typedef struct   SDLW_Calander    SDLW_Calander;
typedef struct   SDLW_ToolBook    SDLW_ToolBook;
typedef struct   SDLW_ToolPage    SDLW_ToolPage;

#endif // SDLW_kernel_h_
