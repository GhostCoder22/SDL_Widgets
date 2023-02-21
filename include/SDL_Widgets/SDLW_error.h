#ifndef SDLW_error_h_
#define SDLW_error_h_

#ifdef __cplusplus
extern "C" {
#endif

#include "SDLW_defines.h"

#define SDLW_E_OK                 0L     /**< No error. */
#define SDLW_E_UNKNOWN_ERROR      2L     /**< Unknown error. */
#define SDLW_E_NOT_INIT           1L     /**< SDL_Widgets not initialized. */
#define SDLW_E_NOT_IMPLEMENTED    3L     /**< Function not implemented. */
#define SDLW_E_INVALID_PARAMETER  5L     /**< Invalid function parameter. */
#define SDLW_E_MEMORY_ERROR       6L     /**< Memory allocation error. */
#define SDLW_E_INVALID_CLASS      7L     /**< Invalid widget class */
#define SDLW_E_TABLE_EMPTY        8L     /**< Data table empty. */
#define SDLW_E_TABLE_FULL         9L     /**< Data table full. */
#define SDLW_E_INVALID_WIDGET     10L    /**< Invalid widget. / Widget not found! */
#define SDLW_E_INVALID_DATA       11L    /**< Invalid data. */
#define SDLW_E_ALREADY_EXISTS     12L    /**< Already exists. */

SDL_dword SDLWAPI SDLW_SetError(SDL_dword errorCode);
SDL_dword SDLWAPI SDLW_GetError(void);
void      SDLWAPI SDLW_ClearError(void);

char*     SDLWAPI SDLW_ErrorToString(SDL_string message, ...);
char*     SDLWAPI SDLW_ErrorToStringEx(SDL_dword errorCode, SDL_string message, ...);

#ifdef __cplusplus
}
#endif
#endif // SDLW_error_h_
