#pragma once

///
/// Properties
///

namespace PROJECT_NAME {

// Information
constexpr auto LibCaption       = "Ultra";
constexpr auto LibDescription   = "Library";
constexpr auto LibRelease       = "2021-07-02";
constexpr auto LibVersion       = "0.0.0";
constexpr auto LibVersionMajor  = 0u;
constexpr auto LibVersionMinor  = 0u;
constexpr auto LibVersionPatch  = 0u;

// Switches
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

// Show platform specific information
#ifdef APP_COMPILER_MSVC
	#pragma message("#> Compiler: " APP_COMPILER " [" APP_COMPILER_VERSION "]")
    #pragma message("#> Platform: " APP_PLATFORM)
    #pragma message("#> PlatformAPI: " APP_PLATFORM_API)
#elif
    // ToDo: Support for other compilers
#endif
