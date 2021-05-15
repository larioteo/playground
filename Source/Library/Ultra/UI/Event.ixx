module;

#include "Ultra/Core.h"

#ifdef APP_PLATFORM_WINDOWS
    //#include "Ultra/Platform/WinAPI/WinWindow.h"
#endif

export module Ultra.UI.Event;
export import Ultra.UI.Event.Data;

import Ultra.Log;

export namespace Ultra {

class EventListener {
public:
    EventListener() = default;
    virtual ~EventListener() = default;


    virtual void Callback(bool &result, void *event = nullptr) = 0;
    virtual void Update() = 0;

    static Scope<EventListener> Create() {
        #ifdef APP_PLATFORM_WINDOWS
            return nullptr;
        #else
            APP_ASSERT(nullptr, "The current platform isn't supported!");
            return nullptr;
        #endif
    }
};

}
