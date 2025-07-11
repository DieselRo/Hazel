#pragma once
#ifndef FMT_HEADER_ONLY
#define FMT_HEADER_ONLY
#endif
#ifdef HZ_PLATFORM_WINDOWS
#ifdef HZ_BUILD_DLL
#define HAZEL_API __declspec(dllexport)
#else
#define HAZEL_API __declspec(dllimport)
#endif
#else
#error Hazel only supports Windows!
#endif

#define BIT(x) (1 << x)