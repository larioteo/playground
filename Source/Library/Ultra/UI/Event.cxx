module;

#include "Ultra/Core.h"

module Ultra.UI.Event;

import Ultra.Log;

#ifdef APP_PLATFORM_WINDOWS
    import Platform.UI.WinAPI.Event;
#endif

namespace Ultra {

Scope<EventListener> EventListener::Create() {
    #ifdef APP_PLATFORM_WINDOWS
        return CreateScope<WinEventListener>();
    #else
        APP_ASSERT(nullptr, "The current platform isn't supported!");
        return nullptr;
    #endif
}

}
