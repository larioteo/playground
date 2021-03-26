#include <Settings.h>

import Library;
import Module;

int main() {
    // Preparation
    #define APP_PLATFORM_WINDOWS1
    #ifdef APP_PLATFORM_WINDOWS
        system("chcp 65001 >nul"); // Switch to UTF-8 codepage
    #endif
    app::log("Hello World!");
    app::LoadModule();
    app::LoadModule();
}
