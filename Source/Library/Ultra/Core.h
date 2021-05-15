#pragma once

#include "Platform.h"
#include "Types.h"

import Ultra.Log;

#define APP_MODE_DEBUG 1
#define APP_DEBUG_MODE 1
#define APP_PROFILE 1

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

///
/// @brief Profiler Extensions
///

#if APP_PROFILE
    #if defined(_MSC_VER) 
        #define APP_FUNCTION_SIGNATURE  __func__
    #endif
    #define APP_PROFILE_BEGIN_SESSION(name, filepath)	::Ultra::Profiler::Instrumentor::Get().BeginSession(name, filepath)
    #define APP_PROFILE_END_SESSION()					::Ultra::Profiler::Instrumentor::Get().EndSession()
    #define APP_PROFILE_SCOPE(name)						constexpr auto fixedName = ::Ultra::Profiler::Utilities::CleanupOutputString(name, "__cdecl "); \
													    ::Ultra::Profiler::Timer timer##__LINE__(fixedName.Data)
    #define APP_PROFILE_FUNCTION()						APP_PROFILE_SCOPE(APP_FUNCTION_SIGNATURE)

    #define APP_PROFILE_SCOPET(name)                    ScopedTimer timer##__LINE__(name, [&](ProfileResult result) { ProfileResults.push_back(result); })
#else
    #define APP_PROFILE_BEGIN_SESSION(name, filepath)
    #define APP_PROFILE_END_SESSION()
    #define APP_PROFILE_SCOPE(name)
    #define APP_PROFILE_FUNCTION()

    #define APP_PROFILE_SCOPET(name)
#endif


///
/// @brief Cleanup
///

#ifdef FROM_PCH
    #include <algorithm>
    #include <cstdint>
    #include <chrono>
    #include <filesystem>
    #include <functional>
    #include <memory>
    #include <mutex>
    #include <random>
    #include <type_traits>
    #include <utility>

    #include <future>
    #include <thread>

    #include <iostream>
    #include <iomanip>
    #include <fstream>
    #include <ostream>
    #include <sstream>

    #include <any>
    #include <array>
    #include <map>
    #include <optional>
    #include <queue>
    #include <string>
    #include <string_view>
    #include <unordered_map>
    #include <unordered_set>
    #include <vector>
#endif
