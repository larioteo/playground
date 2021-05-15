module;

#include "Ultra/Core.h"

#ifdef APP_PLATFORM_WINDOWS
    //#include "Ultra/Platform/WinAPI/WinWindow.h"
#endif

export module Ultra.UI.Window;
export import Ultra.UI.Window.Data;

import Ultra.Log;

namespace Ultra {

export class Window {
public:
    Window() = default;
    virtual ~Window() = default;

    static Scope<Window> Create(const WindowProperties &properties = WindowProperties()) {
        #ifdef APP_PLATFORM_WINDOWS
            return nullptr;
        #else
            APP_ASSERT(nullptr, "The current platform isn't supported!");
            return nullptr;
        #endif
    }

    virtual void Update() = 0;

    // Accessors
    virtual void *GetNativeWindow() = 0;
    virtual const WindowProperties &GetProperties() const = 0;
    virtual const WindowSize GetContexttSize() const = 0;
    virtual const WindowPosition GetDisplayPosition() const = 0;
    virtual const WindowSize GetDisplaySize() const = 0;
    virtual const WindowSize GetScreenSize() const = 0;
    virtual const bool GetState() const = 0;
    virtual const string GetTitle() const = 0;

    // Modifiers
    virtual void SetProperties(const WindowProperties &properties) = 0;
    virtual void SetCursorPosition(const int32_t x, const int32_t y) = 0;
    virtual void SetDisplayPosition(const int32_t x, const int32_t y) = 0;
    virtual void SetProgress(const float progress) = 0;
    virtual void SetTitle(const string_view title) = 0;
};

}
