#include <SDL_Widgets/SDLW_error.h>

SDL_dword m_SDLW_ErrorCode = 0;

SDL_dword SDLW_SetError(SDL_dword errorCode)
{
  SDL_dword result = m_SDLW_ErrorCode;
  m_SDLW_ErrorCode = errorCode;
  return result;
}

SDL_dword SDLW_GetError(void)
{
  return m_SDLW_ErrorCode;
}

void SDLW_ClearError(void)
{
  m_SDLW_ErrorCode = 0;
}


char* SDLW_ErrorToString(SDL_string message, ...)
{
  return NULL;
}

char* SDLW_ErrorToStringEx(SDL_dword errorCode, SDL_string message, ...)
{
  return NULL;
}
