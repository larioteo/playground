module;

#include "Core.h"
#include "Ultra.inl"

export module Ultra;

// Prime Extensions
export import Ultra.Config;
export import Ultra.Log;

// Core Extensions
export import Ultra.Application;
export import Ultra.Core.Layer;

// System Extensions
export import Ultra.System.Cli;
export import Ultra.System.FileSystem;
export import Ultra.System.Input;

// UI Extensions
export import Ultra.UI.Event;
export import Ultra.UI.Window;

// Utility Extensions
export import Ultra.Utility.DateTime;
export import Ultra.Utility.Delegate;
export import Ultra.Utility.Enum;
export import Ultra.Utility.Future;
export import Ultra.Utility.Property;
export import Ultra.Utility.Random;
export import Ultra.Utility.String;
export import Ultra.Utility.ThreadPool;
export import Ultra.Utility.Timer;
export import Ultra.Utility.UUID;

export namespace Ultra {

void ShowLibraryInfo() {
    logger << "Library Information\n"
        << " - Caption:     " << LibCaption     << "\n"
        << " - Description: " << LibDescription << "\n"
        << " - Release:     " << LibRelease     << "\n"
        << " - Version:     " << LibVersion     << "\n";

    logger << "Library Features\n" << std::boolalpha
        << " - Prime:   " << Features::LibPrimeExtensions   << "\n"
        << " - Core:    " << Features::LibCoreExtensions    << "\n"
        << " - Debug:   " << Features::LibDebugExtensions   << "\n"
        << " - GFX:     " << Features::LibGfxExtensions     << "\n"
        << " - System:  " << Features::LibSystemExtensions  << "\n"
        << " - UI:      " << Features::LibUiExtensions      << "\n"
        << " - Utility: " << Features::LibUtilityExtensions << "\n"
        << std::noboolalpha;

    logger << "\n";
}

}
