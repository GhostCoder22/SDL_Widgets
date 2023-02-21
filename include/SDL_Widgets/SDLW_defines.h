#ifndef SDLW_defines_h_
#define SDLW_defines_h_

#include <stdlib.h>
#include <stdio.h>
#ifndef __cplusplus
#include <stdbool.h>
#endif
#include <limits.h>
#include <ctype.h>
#include <wchar.h>
#include <tchar.h>
#include <math.h>

#if defined(WINDOWS) || defined(__CYGWIN__)
#include <windows.h>
#include <windowsx.h>
#endif

#include <SDL2/SDL.h>
#ifdef HAVE_VULKAN_H
#include <vulkan/vulkan.h>
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

#ifndef SDL_STDTYPES
typedef          char       SDL_bit;
#define SDL_BIT_MAX         CHAR_BIT;
#define SDL_CHAR_MIN        SCHAR_MIN;
#define SDL_CHAR_MAX        SCHAR_MAX
typedef unsigned char       SDL_byte;
#define SDL_BYTE_MAX        UCHAR_MAX
typedef          short      SDL_short;
#define SDL_SHORT_MIN       SHRT_MIN
#define SDL_SHORT_MAX       SHRT_MAX
typedef unsigned short      SDL_word;
#define SDL_WORD_MIN        USHRT_MAX
typedef          int        SDL_int;
#define SDL_INT_MIN         INT_MIN
#define SDL_INT_MAX         INT_MAX
typedef unsigned int        SDL_Uint;
#define SDL_UINT_MIN        UINT_MAX
typedef          long       SDL_long;
#define SDL_LONG_MIN        LONG_MIN
#define SDL_LONG_MAX        LONG_MAX
typedef unsigned long       SDL_Ulong;
#define SDL_ULONG_MIN       ULONG_MAX
typedef unsigned long       SDL_dword;
#define SDL_DWORD_MAX       ULONG_MAX
typedef          long long  SDL_longlong;
#define SDL_LONGLONG_MIN    LLONG_MAX
#define SDL_LONGLONG_MAX    LLONG_MAX
typedef unsigned long long  SDL_Ulonglong;
#define SDL_ULONGLONG_MAX   ULLONG_MAX

typedef       char     SDL_char;
typedef const char*    SDL_string;
typedef       wchar_t  SDL_Wchar;
typedef const wchar_t* SDL_Wstring;

#define SDL_text(x)   x
#define SDL_Wtext(x)  L##x
#ifdef UNICODE
#define SDL_Ustring  SDL_Wstring
#define SDL_Utext(x) SDL_Wtext(x)
#else
#define SDL_Ustring  SDL_string
#define SDL_Utext(x) SDL_text(x)
#endif // UNICODE
#endif // !SDL_STDTYPES

#ifndef NULL
#ifdef __cplusplus
#define NULL  0
#else
#define NULL  ((void*)0)
#endif
#endif

#define SDLW_L_CODE(c) 0xc##L

#define SDL_Pause \
  printf("\n"); \
  system("PAUSE"); \
  printf("\n");


#endif // SDLW_defines_h_
