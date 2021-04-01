#include <Settings.h>

#include <chrono>
#include <iostream>

import App.System.Cli;
import App.Utility.DateTime;
import App.Utility.Random;
import App.Utility.String;
import App.Utility.Timer;
import App.Utility.UUID;
//import Test;

int main(int argc, char **argv) {
    // Preparation
    #define APP_PLATFORM_WINDOWS1
    #ifdef APP_PLATFORM_WINDOWS
        system("chcp 65001 >nul"); // Switch to UTF-8 codepage
    #endif

    // CommandLine Interface
    app::cli::Test();

    // DateTime
    std::cout << app::clock.GetTimeStamp() << std::endl;
    std::cout << app::clock.GetIsoDate() << std::endl;
    std::cout << app::clock.GetIsoTime() << std::endl;

    //// DynamicLibrary
    //app::LoadModule();

    //// Utilities
    app::String::Test();
    app::UUID uuid;
    std::cout << uuid << std::endl;

    return 0;
}
