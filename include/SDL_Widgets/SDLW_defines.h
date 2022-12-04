#ifndef SDLW_defines_h_
#define SDLW_defines_h_

#endif // SDLWDEF
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

typedef SDLW_Widget SDLW_Widget;

#endif // SDLW_defines_h_
