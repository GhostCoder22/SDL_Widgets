#ifndef SDLW_kernel_h_
#define SDLW_kernel_h_

#include "SDLW_defines.h"

typedef struct SDLW_DeviceContext
{
    enum { SDLW_DC_Surface,            /**< Surface */
           SDLW_DC_Renderer,           /**< Renderer */
           SDLW_DC_GLContext,          /**< OpenGL */
           SDLW_DC_VkSurface } Type;   /**< Vulkan Surface */
    union
    {
      SDL_Surface*   Surface;
      SDL_Renderer*  Renderer;
      SDL_GLContext* GLContext;
     #ifdef HAVE_VULKAN_H
      struct
      {
          VkSurfaceKHR* Surface;
          VkInstance    Instance;
      } Vulkan;
     #endif
    };
} SDLW_DeviceContext;

typedef struct SDLW
{
    SDL_string          AppName;
    Uint32              flags;
    SDLW_DeviceContext  DeviceContext;
} SDLW;

void       SDLWAPI SDLW_Init(SDLW* info);
bool       SDLWAPI SDLW_WasInit(void);
SDL_string SDLWAPI SDLW_GetAppName(void);
bool       SDLWAPI SDLW_Quit(void);

bool SDLWAPI SDLW_SetDeviceContext(SDLW_DeviceContext DeviceContext);
bool SDLWAPI SDLW_GetDeviceContext(SDLW_DeviceContext DeviceContext);
bool SDLWAPI SDLW_QueryDeviceContext(SDLW_DeviceContext DeviceContext);


typedef struct   SDLW_Widget  SDLW_Widget;
typedef SDL_word SDLW_WidgetID;


#define SDLW_T_Dialog       SDL_text("SDLW_T_Dialog")        /**< Dialog */
#define SDLW_T_Button       SDL_text("SDLW_T_Button")        /**< Button */
#define SDLW_T_Menu         SDL_text("SDLW_T_Menu")          /**< Menu */
#define SDLW_T_ToolBar      SDL_text("SDLW_T_ToolBar")       /**< Toolbar */
#define SDLW_T_StatusBar    SDL_text("SDLW_T_StatusBar")     /**< Status Bar */
#define SDLW_T_ScrollBar    SDL_text("SDLW_T_ScrollBar")     /**< Scroll Bar */
#define SDLW_T_ListView     SDL_text("SDLW_T_ListView")      /**< List View */
#define SDLW_T_TreeView     SDL_text("SDLW_T_TreeView")      /**< Tree View */
#define SDLW_T_TabBar       SDL_text("SDLW_T_TabBar")        /**< Tab Bar */
#define SDLW_T_ProgressBar  SDL_text("SDLW_T_ProgressBar")   /**< Progress Bar */
#define SDLW_T_Slider       SDL_text("SDLW_T_Slider")        /**< Slider */
#define SDLW_T_SplitBar     SDL_text("SDLW_T_SplitBar")      /**< Split Bar */
#define SDLW_T_ToolBook     SDL_text("SDLW_T_ToolBook")      /**< Tool Book */
#define SDLW_T_Calander     SDL_text("SDLW_T_Calander")      /**< Date/Time Calender */

typedef struct SDLW_Event
{
    SDLW_Widget* widget;
    SDL_Uint     message;
    SDL_Uint     item;
    void*        data;
} SDLW_Event;

typedef bool (*SDLW_WidgetProc)(SDLW_Event* event);
typedef struct SDLW_Class
{
    SDL_string       className;
    SDLW_WidgetProc  procedure;
    SDL_byte         id;
    SDL_dword        styles;
    void*            data;
} SDLW_Class;

SDL_byte SDLWAPI SDLW_RegisterClass(SDLW_Class* lpwc);
bool     SDLWAPI SDLW_SetClassInfo(SDL_dword flags, SDL_string className, SDLW_Class* info);
SDL_byte SDLWAPI SDLW_GetClassInfo(SDL_string className, SDLW_Class* info);
bool     SDLWAPI SDLW_UnregisterClass(SDL_string className);


typedef struct SDLW_WidgetInfo
{
    SDL_string     className;
    SDLW_WidgetID  id;
    SDL_dword      styles;
    int            x,     y;
    int            width, height;
    SDLW_Widget*   parent;
    void*          widget_data;
    void*          user_data;
} SDLW_WidgetInfo;



SDLW_Widget*  SDLWAPI SDLW_CreateWidget(SDLW_WidgetInfo* lpwc);
SDLW_WidgetID SDLWAPI SDLW_GetWidgetInfo(SDLW_Widget* widget, SDLW_WidgetInfo* info);
bool          SDLWAPI SDLW_DestroyWidget(SDLW_Widget* widget);

SDL_string    SDLWAPI SDLW_GetWidgetClass(SDLW_Widget* widget);
SDLW_WidgetID SDLWAPI SDLW_GetWidgetID(SDLW_Widget* widget);
SDLW_Widget*  SDLWAPI SDLW_GetWidgetParent(SDLW_Widget* widget);
bool          SDLWAPI SDLW_CallWidgetProc(SDLW_Widget* widget);


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
