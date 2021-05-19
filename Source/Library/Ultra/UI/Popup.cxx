module;

#include "Ultra/Core.h"

module Ultra.UI.Popup;

import Ultra.Log;

#ifdef APP_PLATFORM_WINDOWS
    import Platform.UI.WinAPI.Popup;
#endif

namespace Ultra {

Scope<Popup> Popup::Create() {
    #ifdef APP_PLATFORM_WINDOWS
        return CreateScope<WinPopup>();
    #else
        APP_ASSERT(nullptr, "The current platform isn't supported!");
        return nullptr;
    #endif
}

}
