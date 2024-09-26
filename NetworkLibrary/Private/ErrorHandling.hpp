#pragma once

#include "../pch.h"

#ifdef UNICODE
#include <io.h>
#include <fcntl.h>	/* needed for setmode */
#endif //!UNICODE

void consolePrint(LPCTSTR format, ...)
{
	va_list args;
	va_start(args, format);
	LPTSTR buffer;
	DWORD charCount = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_STRING, format, 0, 0, (LPTSTR)&buffer, 0, &args);

	if (charCount)
	{
		HANDLE Out = GetStdHandle(STD_OUTPUT_HANDLE);
		BOOL success = WriteConsole(Out, buffer, charCount, nullptr, NULL);

		if (0 == success)
		{
			/*int error = */GetLastError();
			DebugBreak();
			OutputDebugString(TEXT("WriteConsole error\n")); /* Used for the debugbreak, to not go out of the scope */
		}
	}
	else
	{
		/*int error = */GetLastError();
		DebugBreak();
		OutputDebugString(TEXT("WriteConsole error\n")); /* Used for the debugbreak, to not go out of the scope */
	}

	LocalFree(buffer);
}

void reportWindowsError(LPCTSTR context, DWORD error)
{
	DWORD dwLangid = 0;
#if 1
#if 0
	dwLangid = MAKELANGID(LANG_FRENCH, SUBLANG_FRENCH);
#else
	DWORD temp;
	if (GetLocaleInfoEx(L"Fr-FR", LOCALE_ILANGUAGE | LOCALE_RETURN_NUMBER, (LPWSTR)&temp, sizeof(temp) / sizeof(WCHAR)))
	{
		dwLangid = temp;
	}
#endif	//!0
#endif	//!1

	LPTSTR buffer;

	DWORD charCount = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, error, dwLangid, (LPTSTR)&buffer, 0, nullptr);

	if (charCount)
	{
		//_tprintf(TEXT("%s: %s"), context, buffer);
		consolePrint(TEXT("%1: %2"), context, buffer);
	}
	else
	{
		//_tprintf(TEXT("%s: error %d\nFormatMessage: error %d\nLook for msdn error codes to find out the issue.\n"), 
		//	context, error, GetLastError());
		consolePrint(TEXT("%1: error %2!d!\nFormatMessage: error %3!d!\nLook for msdn error codes to find out the issue.\n"),
			context, error, GetLastError());
	}

	LocalFree(buffer);
}

//#include <iostream>
//
//int appMain(int argc, TCHAR* argv[])
//{
//	std::cout << "éèê" << std::endl;
//
//#ifdef UNICODE
//	_setmode(_fileno(stdout), _O_U16TEXT);	/* Sets unicode UTF16 character mode */
//#else
//	DWORD codePage;
//	if (GetLocaleInfoEx(L"Fr-FR", LOCALE_IDEFAULTANSICODEPAGE | LOCALE_RETURN_NUMBER, (LPWSTR)&codePage, 2))
//	{
//		SetConsoleOutputCP(1252);	/* Sets code page to western europ latin when in ANSI */
//	}
//	else
//	{
//		reportWindowsError(TEXT("GetLocaleInfoEx"), GetLastError());
//	}
//#endif
//
//	std::cout << "éèê" << std::endl;
//
//	WSADATA data;
//	int result = WSAStartup(MAKEWORD(majorVersion, minorVersion), &data);
//
//	if (result)
//	{
//		reportWindowsError(TEXT("WSAStartup"), result);
//		return Exit_WSAStartup;
//	}
//
//	if (LOBYTE(data.wVersion) == majorVersion
//		&& HIBYTE(data.wVersion) == minorVersion)
//	{
//		consolePrint(TEXT("Network is running, do your stuff here.\n"));
//	}
//	else
//	{
//		consolePrint(TEXT("Incompatible version failure.\n"));
//	}
//
//	result = WSACleanup();
//	if (SOCKET_ERROR == result)
//	{
//		reportWindowsError(TEXT("WSACleanup"), WSAGetLastError());
//		return Exit_WSACleanup;
//	}
//
//	//reportWindowsError(TEXT("test"), 51);	/* Test error message (51 for other message) */
//
//	return 0;
//}
//
//#define _CRTDBG_MAP_ALLOC
//#include <crtdbg.h>
//
//int _tmain(int argc, TCHAR* argv[])
//{
//	_CrtMemState start;
//	_CrtMemCheckpoint(&start);
//
//	int returnValue = appMain(argc, argv);
//
//	_CrtMemState end;
//	_CrtMemCheckpoint(&end);
//
//	_CrtMemState difference;
//	if (_CrtMemDifference(&difference, &start, &end))
//	{
//		OutputDebugString(TEXT("---------- _CrtMemDumpStatistics ----------\n\n"));
//		_CrtMemDumpStatistics(&difference);
//		OutputDebugString(TEXT("\n---------- _CrtMemDumpAllObjectsSince ----------\n\n"));
//		_CrtMemDumpAllObjectsSince(&end);
//		OutputDebugString(TEXT("\n---------- _CrtMemDumpMemoryLeaks ----------\n\n"));
//		_CrtDumpMemoryLeaks();
//
//		if (!returnValue)
//		{
//			return -1;
//		}
//	}
//
//	return returnValue;
//}