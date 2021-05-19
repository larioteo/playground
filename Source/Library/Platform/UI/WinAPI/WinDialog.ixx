module;

#include "Ultra/Core.h"

export module Platform.UI.WinAPI.Popup;

import Ultra.UI.Popup;

export namespace Ultra {

class WinPopup: public Popup {
public:
    WinPopup() = default;
    virtual ~WinPopup() = default;

    string FileDialog(const char *filter) const override;
};

}
