module;

#include "Core.h"
#include "Ultra.inl"

export module Ultra;

// Prime Extensions
export import Ultra.Config;
export import Ultra.Log;

// Core Extensions
export import Ultra.Application;
export import Ultra.Layer;

// System Extensions
export import Ultra.System.Cli;

// Utility Extensions
export import Ultra.Utility.DateTime;
export import Ultra.Utility.Enum;
export import Ultra.Utility.EventData;
export import Ultra.Utility.Random;
export import Ultra.Utility.String;
export import Ultra.Utility.Timer;
export import Ultra.Utility.UUID;

export namespace Ultra {

void ShowLibraryInfo() {
    applog << "Library Information\n"
        << " - Caption:     " << Library::LibCaption << "\n"
        << " - Description: " << Library::LibDescription << "\n"
        << " - Release:     " << Library::LibRelease << "\n"
        << " - Version:     " << Library::LibVersion << "\n"
        << "\n";
}

}
