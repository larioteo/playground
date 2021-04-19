#pragma once

#include "Core.h"

import App.Application;

extern app::Application *CreateApplication();

int main(int argc, char **argv) {
    // Preparation
    #ifdef APP_PLATFORM_WINDOWS
        // Switch to UTF-8 codepage
        system("chcp 65001 >nul");
    #endif

    // Initialization
    auto app = CreateApplication();

    // Main
    app->Run();

    // Termination
    delete app;
    return 0;
}
