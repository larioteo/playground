﻿#pragma once


#ifdef APP_DISABLED_CODE

#include "Ultra/Core.h"

import Ultra.Event;

namespace Ultra {

// Events: https://msdn.microsoft.com/en-us/library/windows/desktop/ms644958%28v=vs.85%29.aspx?f=255&MSPPError=-2147217396
class WinEventListener: public EventListener {
public:
	// Default
	WinEventListener();
	virtual ~WinEventListener();

	// Events
	virtual void Callback(bool &result, void *event = nullptr) override;
	virtual void Update() override;

private:
	intptr_t Register(void *event);
};

}

#endif
