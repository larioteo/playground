#pragma once

// Properties
/// These two lines will select the dedicated graphics card on Laptos with two graphic cards installed.
extern "C" {
	__declspec(dllexport) unsigned long NvOptimusEnablement = 0x1;
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

namespace SOLUTION_NAME {

// Information
constexpr inline auto AppCaption = "App";
constexpr inline auto AppDescription = "-";
constexpr inline auto AppRelease = "2020-07-02";
constexpr inline auto AppVersion = "0.0.0";
constexpr inline auto AppVersionMajor = 0u;
constexpr inline auto AppVersionMinor = 0u;
constexpr inline auto AppVersionPatch = 0u;

// Switches
#ifdef APP_MODE_DEBUG
	constexpr inline bool DebugMode = true;
#else
	constexpr inline bool DebugMode = false;
#endif
#ifdef APP_MODE_RELEASE
	constexpr inline bool ReleaseMode = true;
#else
	constexpr inline bool ReleaseMode = false;
#endif
#ifdef APP_MODE_DISTRIBUTION
	constexpr inline bool DistributionMode = true;
#else
	constexpr inline bool DistributionMode = false;
#endif
#ifdef APP_MODE_VERBOSE
	constexpr inline bool VerboseMode = true;
#else
	constexpr inline bool VerboseMode = false;
#endif

}
