#pragma once

///
/// Library Properties
///

// Switches to enable or disable Features
#define LIB_EXTENSION_PRIME     1    // Configuration and Logging Extensions
#define LIB_EXTENSION_CORE      1    // Applicaiton Layout and Workflow Extensions
#define LIB_EXTENSION_DEBUG     1    // Debugging and Profiling Extension
#define LIB_EXTENSION_GFX       1    // Graphics API selection and Context Binding Extensions
#define LIB_EXTENSION_SYSTEM    1    // System Extensions like color output in cli or input capturing
#define LIB_EXTENSION_UI        1    // UI extensions like window creation and event system
#define LIB_EXTENSION_UTILITIES 1    // Usefull utilities like date and time, properties and timer helpers, etc.

namespace Ultra {

// Information
constexpr auto LibCaption       = "Ultra Library";
constexpr auto LibDescription   = "Game Engine Framework";
constexpr auto LibRelease       = "2021-07-02";
constexpr auto LibVersion       = "0.0.1";
constexpr auto LibVersionMajor  = 0u;
constexpr auto LibVersionMinor  = 0u;
constexpr auto LibVersionPatch  = 1u;

// Features
namespace Features {

#ifdef LIB_EXTENSION_PRIME
    constexpr bool LibPrimeExtensions = true;
#else
    constexpr bool LibPrimeExtensions = false;
#endif
#ifdef LIB_EXTENSION_CORE
    constexpr bool LibCoreExtensions = true;
#else
    constexpr bool LibCoreExtensions = false;
#endif
#ifdef LIB_EXTENSION_DEBUG
    constexpr bool LibDebugExtensions = true;
#else
    constexpr bool LibDebugExtensions = false;
#endif
#ifdef LIB_EXTENSION_GFX
    constexpr bool LibGfxExtensions = true;
#else
    constexpr bool LibGfxExtensions = false;
#endif
#ifdef LIB_EXTENSION_SYSTEM
    constexpr bool LibSystemExtensions = true;
#else
    constexpr bool LibSystemExtensions = false;
#endif
#ifdef LIB_EXTENSION_UI
    constexpr bool LibUiExtensions = true;
#else
    constexpr bool LibUiExtensions = false;
#endif
#ifdef LIB_EXTENSION_UTILITIES
    constexpr bool LibUtilityExtensions = true;
#else
    constexpr bool LibUtilityExtensions = false;
#endif

}

}

// Show platform specific information, the preprocessor value is defined in 'Core.h', therefore it should be included first)
#ifdef APP_COMPILER_MSVC
	#pragma message("#> Compiler: " APP_COMPILER " [" APP_COMPILER_VERSION "]")
    #pragma message("#> Platform: " APP_PLATFORM)
    #pragma message("#> PlatformAPI: " APP_PLATFORM_API)
#elif
    // ToDo: Support for other compilers when they are VS C++20 module ready.
#endif
