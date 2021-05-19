module;

#include "Ultra/Core.h"

export module Ultra.UI.Popup;

export namespace Ultra {

class Popup {
public:
    Popup() = default;
    virtual ~Popup() = default;
    static Scope<Popup> Create();

    virtual string FileDialog(const char *filter = "All\0*.*\0") const = 0;
};

}
