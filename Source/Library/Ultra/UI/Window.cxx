module;

#include "Ultra/Core.h"

module Ultra.UI.Window;

import Ultra.Log;

#ifdef APP_PLATFORM_WINDOWS
    import Platform.UI.WinAPI.Window;
#endif

namespace Ultra {

Scope<Window> Window::Create(const WindowProperties &properties) {
    #ifdef APP_PLATFORM_WINDOWS
        return CreateScope<WinWindow>(properties);
    #else
        APP_ASSERT(nullptr, "The current platform isn't supported!");
        return nullptr;
    #endif
}

}
