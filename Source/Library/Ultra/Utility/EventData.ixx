module;

#include <cstdint>
#include <functional>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

export module App.Utility.EventData;

import App.Utility.Enum;

///
/// @brief EventData Interface and EventPriority
/// 
export namespace app { namespace utility {

// Enumerations
enum class EventCategory {
    Default     = 0x00,
    System      = 0x10,
    Application = 0xA0,
    Library     = 0xB0,
    User        = 0xC0,
    Undefined   = 0xF0,
};
inline std::ostream &operator<<(std::ostream &stream, app::utility::EventCategory category) {
    switch (category) {
        case app::utility::EventCategory::Default:		{ stream << "Default";		break; }
        case app::utility::EventCategory::System:		{ stream << "System";	    break; }
        case app::utility::EventCategory::Application:	{ stream << "Application";	break; }
        case app::utility::EventCategory::Library:		{ stream << "Library";		break; }
        case app::utility::EventCategory::User:		    { stream << "User";		    break; }
        default:                                        { stream << "Undefined";    break; }
    }
    return stream;
}

enum class EventPriority {
    Default     = 0x00,
    Low         = 0x01,
    Normal      = 0x03,
    High        = 0x05,
    Realtime    = 0x07,
    Undefined   = 0x0F,
};
inline std::ostream &operator<<(std::ostream &stream, app::utility::EventPriority priority) {
    switch (priority) {
        case app::utility::EventPriority::Default:		{ stream << "Default";		break; }
        case app::utility::EventPriority::Low:			{ stream << "Low";			break; }
        case app::utility::EventPriority::Normal:		{ stream << "Normal";		break; }
        case app::utility::EventPriority::High:			{ stream << "High";		    break; }
        case app::utility::EventPriority::Realtime:		{ stream << "Realtime";	    break; }
        default:                                        { stream << "Undefined";    break; }
    }
    return stream;
}

enum class EventSource {
    // Default
    Default		= 0x01,

    // System
    Device		= 0x11,
    Power		= 0x12,

    // Application
    Window		= 0xA1,
    Context		= 0xA2,
    Keyboard	= 0xA3,
    Mouse		= 0xA4,
    Touch		= 0xA5,
    Controller	= 0xA6,

    // Library
    Module  	= 0xB1,

    // User
    User    	= 0xC1,

    // ~
    Undefined	= 0xFF,
};
inline std::ostream &operator<<(std::ostream &stream, app::utility::EventSource source) {
    switch (source) {
        case app::utility::EventSource::Default:	{ stream << "Default";		break; }
        case app::utility::EventSource::Device:		{ stream << "Device";		break; }
        case app::utility::EventSource::Power:		{ stream << "Power";		break; }
        case app::utility::EventSource::Window:		{ stream << "Window";		break; }
        case app::utility::EventSource::Keyboard:	{ stream << "Keyboard";		break; }
        case app::utility::EventSource::Mouse:		{ stream << "Mouse";		break; }
        case app::utility::EventSource::Touch:		{ stream << "Touch";		break; }
        case app::utility::EventSource::Controller: { stream << "Controller";	break; }
        case app::utility::EventSource::Context:	{ stream << "Context";		break; }
        default:                                    { stream << "Undefined";    break; }
    }
    return stream;
}

// Helper
inline bool IsEventCategory(app::utility::EventSource source, app::utility::EventCategory category) {
    if (app::utility::GetEnumType(source) & 1 << app::utility::GetEnumType(category)) {
        return true;
    }
    return false;
}

/// Interfaces
// Event-Data
struct IEventData {
    // Constructors and Deconstructor
    IEventData() = delete;
    IEventData(EventSource source, EventPriority priority = EventPriority::Normal): Source(source), Priority(priority) {
        if (IsEventCategory(source, EventCategory::System)) {
            Category = EventCategory::System;
        } else if (IsEventCategory(source, EventCategory::Application)) {
            Category = EventCategory::Application;
        } else if (IsEventCategory(source, EventCategory::Library)) {
            Category = EventCategory::Library;
        } else if (IsEventCategory(source, EventCategory::User)) {
            Category = EventCategory::User;
        }
    };
    IEventData(const std::string_view message, EventPriority priority = EventPriority::Normal, EventSource source = EventSource::User): Source(source), Priority(priority) {};
    virtual ~IEventData() = default;

    // Conversions
    template<typename T>
    T *As() {
        return reinterpret_cast<T *>(this);
    }

    // Properties
    bool Handled = false;
    std::string Message = {};
    EventCategory Category = EventCategory::Undefined;
    EventPriority Priority = EventPriority::Undefined;
    EventSource Source = EventSource::Undefined;
};
inline std::ostream &operator<<(std::ostream &stream, IEventData data) {
    stream << "Base Event Data\n - Category: " << data.Category << "\n - Source:   " << data.Source << "\n - Priority: " << data.Priority << std::endl;
    return stream;
}


}}

