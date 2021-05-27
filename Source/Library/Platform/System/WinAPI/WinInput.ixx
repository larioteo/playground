module;

#include "Ultra/Core.h"

export module Platform.System.Input;

import Ultra.System.Input;
import Ultra.UI.Event.Data;

export namespace Ultra {

class WinInput: public Input {
protected:
	virtual bool GetKeyStatePlatform(KeyCode code) const override;
	virtual bool GetMouseButtonStatePlatform(MouseButton button) const override;
	virtual std::pair<float, float> GetMousePositionPlatform() const override;
};

}
