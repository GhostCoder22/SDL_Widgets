#ifndef SDLW_kernel_h_
#define SDLW_kernel_h_

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef NULL
#ifdef __cplusplus
#define NULL  0
#else
#define NULL  ((void*)0)
#endif
#endif

#ifndef SDLWAPI
# if defined(WINDOWS) || defined(__CYGWIN__)
#  ifdef BUILDLIB
#   define SDLWAPI __declspec(dllexport)
#  else
#   define SDLWAPI __declspec(dllimport)
#  endif
# elif defined(__OS2__)
#   ifdef BUILDLIB_SDLP
#    define SDLWAPI __declspec(dllexport)
#   else
#    define SDLWAPI __declspec(dllimport)
#   endif
# else
#  if defined(__GNUC__) && __GNUC__ >= 4
#   define SDLWAPI __attribute__ ((visibility("default")))
#  else
#   define SDLWAPI
#  endif
# endif
#endif

typedef struct SDL_Widget SDL_Widget;

SDL_Widget* SDLWAPI SDLW_CreateWidget(const char* title);
const char* SDLWAPI SDLW_SetWidgetTitle(SDL_Widget* widget, const char* newTitle);
const char* SDLWAPI SDLW_GetWidgetTitle(SDL_Widget* widget);
bool        SDLWAPI SDLW_DestroyWidget(SDL_Widget* widget);

#endif // SDLW_kernel_h_
