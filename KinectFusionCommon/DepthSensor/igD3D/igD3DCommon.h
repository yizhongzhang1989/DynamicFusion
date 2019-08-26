#pragma once

// Usage: igD3D Common Header
// Created: 2018.4.28
// 2018.4.28: Initial Version, renamed from igD3DUtls.h.
// 2019.3.4: Added IG_D3D_FALSE_RETURN macro.
// 2019.4.9: Added d3dcommon.h and dxguid.lib pragma comment.
// Version: 1.0.190409.1900

#ifdef _DEBUG
#define _DUMP_DEBUG_MESSAGE_
#endif

#define _DUMP_ERROR_MESSAGE_TO_STD_OUT

#include <windows.h>
#ifdef _DUMP_ERROR_MESSAGE_TO_STD_OUT
#include <iostream>
#endif

#include <d3dcommon.h>
#pragma comment(lib, "dxguid.lib")

namespace ig
{
	namespace D3D
	{
		inline void PrintMessage(const wchar_t *msg)
		{
#ifdef _DUMP_ERROR_MESSAGE_TO_STD_OUT
			::std::wcout << msg;
#endif
			OutputDebugStringW(msg);
		}
		inline void DumpMessage(const wchar_t* format, ...)
		{
			wchar_t buffer[256];
			va_list ap;
			va_start(ap, format);
			vswprintf(buffer, 256, format, ap);
			PrintMessage(buffer);
			PrintMessage(L"\n");
		}
		inline void DumpMessage()
		{
		}
	}
}


#ifdef _DUMP_DEBUG_MESSAGE_

#define IG_TOSTRING0(x) #x
#define IG_TOSTRING(x) IG_TOSTRING0(x)
#define IG_TOWIDE0(x) L##x
#define IG_TOWIDE(x) IG_TOWIDE0(x)

#define IG_D3D_ASSERT(x, ... ) {\
if (!(x))\
{ ig::D3D::DumpMessage(L"\nAsseration failed in " IG_TOWIDE(IG_TOSTRING(__FILE__)) L" : Line " IG_TOWIDE(IG_TOSTRING(__LINE__))); \
	ig::D3D::DumpMessage(__VA_ARGS__); \
	__debugbreak(); }}

#define IG_D3D_ASSERT_SUCCEEDED(hr, ... ) {\
HRESULT hResultForAssertSucceeded;\
if (FAILED(hResultForAssertSucceeded = (hr)))\
{ ig::D3D::DumpMessage(L"\nHRESULT failed in " IG_TOWIDE(IG_TOSTRING(__FILE__)) L" : Line " IG_TOWIDE(IG_TOSTRING(__LINE__))); \
	ig::D3D::DumpMessage(L"hr = 0x%08X", hResultForAssertSucceeded); \
	ig::D3D::DumpMessage(__VA_ARGS__); \
	__debugbreak(); }}

#define IG_D3D_ERROR_DUMP( ... ) {\
	ig::D3D::DumpMessage(L"\nError occurs in " IG_TOWIDE(IG_TOSTRING(__FILE__)) L" : Line " IG_TOWIDE(IG_TOSTRING(__LINE__))); \
	ig::D3D::DumpMessage(__VA_ARGS__); }

#define IG_D3D_DEBUG_DUMP(msg, ...) {\
	ig::D3D::DumpMessage(msg); \
	ig::D3D::DumpMessage(__VA_ARGS__); }

#define IG_D3D_FAILED_RETURN(hr, ... ) {\
HRESULT hResultForFailedReturn;\
if (FAILED(hResultForFailedReturn=(hr)))\
{ ig::D3D::DumpMessage(L"\nHRESULT failed in " IG_TOWIDE(IG_TOSTRING(__FILE__)) L" : Line " IG_TOWIDE(IG_TOSTRING(__LINE__))); \
	ig::D3D::DumpMessage(L"hr = 0x%08X", hResultForFailedReturn); \
	ig::D3D::DumpMessage(__VA_ARGS__); \
	return hResultForFailedReturn; }}

#define IG_D3D_FALSE_RETURN(x, ... ) {\
if (!(x))\
{ ig::D3D::DumpMessage(L"\nHRESULT failed in " IG_TOWIDE(IG_TOSTRING(__FILE__)) L" : Line " IG_TOWIDE(IG_TOSTRING(__LINE__))); \
	ig::D3D::DumpMessage(__VA_ARGS__); \
	return false; }}

#else
#define IG_D3D_ASSERT(x, ... ) {(x);}
#define IG_D3D_ASSERT_SUCCEEDED(hr, ... ) {(hr);}

#define IG_D3D_FAILED_RETURN(hr, ... ) {\
HRESULT hResultForFailedReturn;\
if (FAILED(hResultForFailedReturn=(hr)))\
{ return hResultForFailedReturn; }}

#define IG_D3D_FALSE_RETURN(x, ... ) {\
if (!(x))\
{ return false; }}

#define IG_D3D_ERROR_DUMP( ... ) {(void)0;}
#define IG_D3D_DEBUG_DUMP(msg, ...) {(void)0;}
#endif

#define IG_D3D_DISABLE_COPY_ASSIGN(clsname) \
	private: \
	clsname(const clsname&); \
	clsname& operator=(const clsname);