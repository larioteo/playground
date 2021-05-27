module;

#include "Ultra/Core.h"

export module Ultra.UI.Event;
export import Ultra.UI.Event.Data;
export import Ultra.Utility.Delegate;
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

    //// System Events
    //Utility::Subject<bool, DeviceEventData &> DeviceEvent;
    //Utility::Subject<bool, PowerEventData &> PowerEvent;

    //// Applicaiton Events
    //Utility::Subject<bool, WindowEventData &> WindowEvent;
    //Utility::Subject<bool, ContextEventData &> ContextEvent;

    //// User Events
    //Utility::Subject<bool, ControllerEventData &> ControllerEvent;
    //Utility::Subject<bool, KeyboardEventData &> KeyboardEvent;
    //Utility::Subject<bool, MouseEventData &> MouseEvent;
    //Utility::Subject<bool, TouchEventData &> TouchEvent;
};

/// Interfaces
// Event-Data
struct Event {
    // Constructors and Deconstructor
    Event() = delete;
    Event(std::shared_ptr<EventData> &&data): Data(std::move(data)) {};
    virtual ~Event() = default;

    // Conversions
    template<typename T>
    T *As() {
        return reinterpret_cast<T *>(this);
    }

    // Properties
    std::shared_ptr<EventData> Data = nullptr;
    bool Handled = false;
};

}
