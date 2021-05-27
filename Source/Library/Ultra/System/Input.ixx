module;

#include <tuple>

#include "Ultra/Core.h"

export module Ultra.System.Input;

import Ultra.UI.Event.Data;

export namespace Ultra {

class Input {
public:
    static Scope<Input> Create();

    static bool GetKeyState(KeyCode code);
    static bool GetMouseButtonState(MouseButton button);
    static std::pair<float, float> GetMousePosition();

protected:
    virtual bool GetKeyStatePlatform(KeyCode code) const = 0;
    virtual bool GetMouseButtonStatePlatform(MouseButton button) const = 0;
    virtual std::pair<float, float> GetMousePositionPlatform() const = 0;

private:
    static Scope<Input> Instance;
};

}
