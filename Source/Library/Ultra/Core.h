#pragma once

#include "Platform.h"
#include "Types.h"

import Ultra.Log;

#define APP_MODE_DEBUG 1
#define APP_DEBUG_MODE 1

///
/// @brief Library Extensions
///

#ifdef APP_PLATFORM_WINDOWS
    #ifdef APP_SHARED_LIBRARY
        #define APP_API __declspec(dllexport)
    #else
        #define APP_API __declspec(dllimport)
    #endif
#else
	#error "This library currently supports only Windows!""
#endif

#ifdef APP_DEBUG_MODE
	#if defined(APP_PLATFORM_WINDOWS)
		#define APP_DEBUGBREAK() __debugbreak()
	#elif defined(APP_PLATFORM_LINUX)
		#include <signal.h>
		#define APP_DEBUGBREAK() raise(SIGTRAP)
	#else
		#define APP_DEBUGBREAK()
		#pragma message("#> Core: Platform doesn't support debug break!")
	#endif
#else
	#define APP_DEBUGBREAK()
#endif

///
/// @brief Logger Extensions (cause performance matters ... sometimes)
///

// Old-School: If anybody wishes preprocessor macros, we have no problem with it.
#define APP_LOG(...)			AppLog			("[", __FUNCTION__, "]: ", __VA_ARGS__)
#define APP_LOG_TRACE(...)      AppLogTrace		("[", __FUNCTION__, "]: ", __VA_ARGS__)
#define APP_LOG_DEBUG(...)      AppLogDebug		("[", __FUNCTION__, "]: ", __VA_ARGS__)
#define APP_LOG_INFO(...)		AppLogInfo		("[", __FUNCTION__, "]: ", __VA_ARGS__)
#define APP_LOG_WARN(...)		AppLogWarning	("[", __FUNCTION__, "]: ", __VA_ARGS__)
#define APP_LOG_ERROR(...)		AppLogError		("[", __FUNCTION__, "]: ", __VA_ARGS__)
#define APP_LOG_FATAL(...)	    AppLogFatal	    ("[", __FUNCTION__, "]: ", __VA_ARGS__)

#ifdef APP_MODE_DEBUG
    #define AppAssert(x, ...) if(AppAssert(x, __VA_ARGS__)) APP_DEBUGBREAK() // Workaround, add the debug break after the message.

    #define APP_ASSERT(x, ...) { if(!(x)) { AppLogFatal("[", __FUNCTION__, "]: ", __VA_ARGS__); APP_DEBUGBREAK(); } }
#elif APP_MODE_RELEASE
    #ifdef LIB_SETTING_REPLACE_LOGGER
        #define AppAssert(...);
 
        #define AppLogDebug(...);
        #define AppLogTrace(...);
    #endif
    
    #define APP_ASSERT(x, ...);

    #define APP_LOG_DEBUG(...);
    #define APP_LOG_TRACE(...);
#elif APP_MODE_DISTRIBUTION
    #ifdef LIB_SETTING_REPLACE_LOGGER
        #define AppAssert(...);
 
        #define AppLogInfo(...);
        #define AppLogDebug(...);
        #define AppLogTrace(...);
    #endif
    
    #define APP_ASSERT(x, ...);

    #define APP_LOG_INFO(...);
    #define APP_LOG_DEBUG(...);
    #define APP_LOG_TRACE(...);
#endif
