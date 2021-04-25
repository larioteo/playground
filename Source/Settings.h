#pragma once

#include <Ultra/Core.h>

///
/// Properties
///

namespace SOLUTION_NAME {

// Information
constexpr auto AppCaption        = "Playground";
constexpr auto AppDescription    = "C++ 20 Playground";
constexpr auto AppRelease        = "2021-07-02";
constexpr auto AppVersion        = "0.0.0";
constexpr auto AppVersionMajor   = 0u;
constexpr auto AppVersionMinor   = 0u;
constexpr auto AppVersionPatch   = 0u;

// Switches
#ifdef APP_MODE_DEBUG
    constexpr bool DebugMode = true;
#else
    constexpr bool DebugMode = false;
#endif
#ifdef APP_MODE_RELEASE
    constexpr bool ReleaseMode = true;
#else
    constexpr bool ReleaseMode = false;
#endif
#ifdef APP_MODE_DISTRIBUTION
    constexpr bool DistributionMode = true;
#else
    constexpr bool DistributionMode = false;
#endif

}

// These two lines will select the dedicated graphics card on Laptos with two graphic cards installed.
extern "C" {
    __declspec(dllexport) unsigned long NvOptimusEnablement = 0x1;
    __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}
