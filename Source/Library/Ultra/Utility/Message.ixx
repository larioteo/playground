module;

#include <cstdint>
#include <functional>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

export module Ultra.Utility.EventData;

import Ultra.Utility.Enum;

///
/// @brief EventData Interface and EventPriority
/// 
export namespace Ultra { namespace Utility {

// Enumerations
enum class EventCategory {
    Default     = 0x00,
    System      = 0x10,
    Application = 0xA0,
    Library     = 0xB0,
    User        = 0xC0,
    Undefined   = 0xF0,
};
inline std::ostream &operator<<(std::ostream &stream, Ultra::Utility::EventCategory category) {
    switch (category) {
        case Ultra::Utility::EventCategory::Default:		{ stream << "Default";		break; }
        case Ultra::Utility::EventCategory::System:		    { stream << "System";	    break; }
        case Ultra::Utility::EventCategory::Application:	{ stream << "Application";	break; }
        case Ultra::Utility::EventCategory::Library:		{ stream << "Library";		break; }
        case Ultra::Utility::EventCategory::User:		    { stream << "User";		    break; }
        default:                                            { stream << "Undefined";    break; }
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
inline std::ostream &operator<<(std::ostream &stream, Ultra::Utility::EventPriority priority) {
    switch (priority) {
        case Ultra::Utility::EventPriority::Default:		{ stream << "Default";		break; }
        case Ultra::Utility::EventPriority::Low:			{ stream << "Low";			break; }
        case Ultra::Utility::EventPriority::Normal:		    { stream << "Normal";		break; }
        case Ultra::Utility::EventPriority::High:			{ stream << "High";		    break; }
        case Ultra::Utility::EventPriority::Realtime:		{ stream << "Realtime";	    break; }
        default:                                            { stream << "Undefined";    break; }
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
inline std::ostream &operator<<(std::ostream &stream, Ultra::Utility::EventSource source) {
    switch (source) {
        case Ultra::Utility::EventSource::Default:	    { stream << "Default";		break; }
        case Ultra::Utility::EventSource::Device:		{ stream << "Device";		break; }
        case Ultra::Utility::EventSource::Power:		{ stream << "Power";		break; }
        case Ultra::Utility::EventSource::Window:		{ stream << "Window";		break; }
        case Ultra::Utility::EventSource::Keyboard:	    { stream << "Keyboard";		break; }
        case Ultra::Utility::EventSource::Mouse:		{ stream << "Mouse";		break; }
        case Ultra::Utility::EventSource::Touch:		{ stream << "Touch";		break; }
        case Ultra::Utility::EventSource::Controller:   { stream << "Controller";	break; }
        case Ultra::Utility::EventSource::Context:	    { stream << "Context";		break; }
        default:                                        { stream << "Undefined";    break; }
    }
    return stream;
}

// Helper
inline bool IsEventCategory(Ultra::Utility::EventSource source, Ultra::Utility::EventCategory category) {
    if (Ultra::Utility::GetEnumType(source) & 1 << Ultra::Utility::GetEnumType(category)) {
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

