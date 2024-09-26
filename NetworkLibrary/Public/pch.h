#pragma once

#ifdef _UNICODE
#undef _UNICODE
#endif //!_UNICODE

#ifdef UNICODE
#undef UNICODE
#endif //!UNICODE

#include <string>

#if 1	/* 0 == ANSI && 1 == UNICODE */
#define _UNICODE
#define UNICODE
#define TSTR std::wstring
#else
#define TSTR std::string
#endif

#include <tchar.h>
#define WIN32_LEAN_AND_MEAN	/* No need for every windows header */
#include <Windows.h>
