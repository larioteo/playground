module;

#include "Ultra/Core.h"

export module Ultra.UI.Event;
import Ultra.UI.Event.Data;

export namespace Ultra {

class EventListener {
public:
    // Default
    EventListener() = default;
    virtual ~EventListener() = default;
    static Scope<EventListener> Create();

    // Methods
    virtual void Callback(bool &result, void *event = nullptr) = 0;
    virtual void Update() = 0;
};

}
