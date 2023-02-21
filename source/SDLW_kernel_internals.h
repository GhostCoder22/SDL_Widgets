#ifndef SDLW_kernel_internals_h_
#define SDLW_kernel_internals_h_

#include "../include/SDL_Widgets/SDLW_kernel.h"

#ifdef __cplusplus
extern "C" {
#endif

struct SDLW_Button
{
  SDL_string  label;
  SDL_Uint    id;
  void*       data;
};


struct SDLW_MenuItem
{
  SDL_string  label;
  SDL_Uint    id;
  void*       data;
  struct SDLW_MenuItem *pNext,*pPrev;
};

struct SDLW_Menu
{
  struct SDLW_MenuItem* table;
  SDLW_MenuID           entries;
  struct SDLW_MenuItem* active;
};


struct SDLW_Tool
{
  SDL_string   label;
  SDLW_ToolID  id;
  void*        data;
  struct SDLW_Tool *pNext,*pPrev;
};

struct SDLW_Toolbar
{
  struct SDLW_Tool* table;
  SDLW_ToolID       tools;
  struct SDLW_Tool* active;
};


struct SDLW_StatusArea
{
  SDL_string   text;
  SDLW_ToolID  id;
  void*        data;
  struct SDLW_StatusArea* pNext;
};

struct SDLW_StatusBar
{
  struct SDLW_StatusArea* table;
  SDLW_ToolID             areas;
};


struct SDLW_ListView_
{
  SDL_string  label;
  SDL_Uint    id;
  void*       data;
  struct SDLW_ListView_ *pNext,*pPrev;
};

struct SDLW_ListView
{
  struct SDLW_ListView_* table;
  SDL_Uint               items;
  struct SDLW_ListView_* active;
};


struct SDLW_TreeView_
{
  SDL_string             label;
  SDL_Uint               id;
  void*                  data;
  struct SDLW_TreeView_* Branch;
  struct SDLW_TreeView_ *pNext,*pPrev;
};

struct SDLW_TreeView
{
  struct SDLW_TreeView_* table;
  SDL_Uint               items;
  struct SDLW_TreeView_* active;
};


struct SDLW_Tab
{
  SDL_string  table;
  SDLW_TabID  id;
  void*       data;
  struct SDLW_Tab *pNext,*pPrev;
};

struct SDLW_TabBar
{
  struct SDLW_Tab* table;
  SDLW_TabID       tabs;
  struct SDLW_Tab* active;
};


struct SDLW_ProgressBar
{
  SDL_string  label;
  int         value;
  float       step;
};


struct SDLW_Slider
{
  SDL_string  label;
  int         value, step;
};


struct SDLW_Calander
{
  SDL_Uint  flags;
  struct
  {
      SDL_byte  month, day;
      SDL_word  year;
  } Date;
  struct
  {
      SDL_byte  millisecond, second, minute, hour;
  } Time;
};


struct SDLW_ToolPage
{
  SDL_string  label;
  SDL_Uint    id;
  void*       data;
  SDLW_ToolPage *pNext,*pPrev;
};

struct SDLW_ToolBook
{
  struct SDLW_ToolPage* table;
  SDLW_ToolID           tools;
};


struct SDLW_Class_
{
   struct SDLW_Class   info;
   struct SDLW_Class_* pNext;
};

struct SDLW_Widget
{
  struct SDLW_Class_* Class;
  SDL_Uint            event, item;
  SDL_dword           styles;
  SDLW_WidgetID       id;
  SDL_Rect            rect;
  struct SDLW_Widget* parent;
  void*               widget_data;
  void*               user_data;
  struct SDLW_Widget *pNext,*pPrev;
};


typedef struct SDLW_
{
    bool                fWasInit;
    SDLW                Info;
    struct SDLW_Class_* ClassTable;
    SDL_byte            Classes;
    struct SDLW_Widget* WidgetTable;
    SDLW_WidgetID       Widgets;
} SDLW_;

#ifdef __cplusplus
}
#endif
#endif // SDLW_kernel_internals_h_
