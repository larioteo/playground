﻿module;

#include <atomic>
#include <filesystem>
#include <mutex>
#include <fstream>
#include <iostream>
#include <memory>
#include <ostream>
#include <string>
#include <string_view>
#include <sstream>
#include <type_traits>

#include "Core.h"

export module Ultra.Log;

import Ultra.System.Cli;
import Ultra.Utility.DateTime;
import Ultra.Utility.String;

using std::is_same_v;
using std::mutex;
using std::ofstream;
using std::ostream;
using std::string;
using std::stringstream;

using namespace std::string_literals;

export namespace Ultra {

// Enumerations
enum class LogLevel {
    // Log Levels
    Trace		= 0x1,	// Message (Level 1): Everything that could be interesting in the future.
    Debug		= 0x2,	// Message (Level 2): Debugging messages, which can help at parts, where we are unsure if the code will ever fail.
    Info		= 0x3,	// Message (Level 3): Information that is maybe usefull for gattering basic system data.
    Warn		= 0x4,	// Message (Level 4): Has no impact on the application itself but should get sometime fixed.
    Error		= 0x5,  // Message (Level 5): Error messages that have no impact on runtime execution.
    Fatal   	= 0x6,	// Message (Level 6): Critical messages which will break runtime execution.
    Default		= 0x7,	// Message (Level 7): Default output with nothing added

    // Conversations
    Critical    = Fatal,

    // Types
    Caption		= 0x8,	// Structure the log by a given caption
    Delimiter	= 0x9,	// Structure the log with a delimiter
};

// Concepts
template <typename T>
concept typename_logmodifier = std::is_same_v<T, LogLevel>;

// Overrides
template <typename_logmodifier T>
inline std::ostream &operator<<(std::ostream &os, T type) {
    return os << static_cast<size_t>(type);
}

///
/// @brief This logging class fullfills basic needs, it takes everything you throw at it (like cout), and offers you some nice colors and similar features like that.
///
class Log {
    // Constructors and Deconstructor
    Log() { if (mLogLevel > LogLevel::Fatal) mLogLevel = LogLevel::Fatal; }
    Log(const Log &) = delete;
    Log(Log &&) noexcept = delete;
    ~Log() = default;

    // Operators
    Log &operator=(const Log &) { return Instance(); }
    Log &operator=(const Log &&) noexcept { return Instance(); }

public:
    template <typename T>
    Log &operator<<(const T &data) {
        std::unique_lock<mutex> lock(mSync);
        if (mSkip) return (*this);

        if constexpr (is_string_v<T>) {
            if (String::EndsWith(data, "\n")) {
                if (mCaptionActive) {
                    mCaptionActive = false;
                    mStream << "\n▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬";
                }

                Reset();
                mCounter++;
            }
        }

        mStream << data;
        return (*this);
    }
    
    template <typename_logmodifier T>
    Log &operator<<(const T &data) {
        std::unique_lock<mutex> lock(mSync);
        
        if constexpr (std::is_same_v<T, LogLevel>) {
            if (data >= mLogLevel) {
                mSkip = false;
                switch (data) {
                    case LogLevel::Fatal:	    { mStream << Cli::Color::Gray << apptime.GetTimeStamp("%Y-%m-%dT%H:%M") << " | " << Cli::Color::Red				<< "[ Fatal ] ";    break; }
                    case LogLevel::Error:       { mStream << Cli::Color::Gray << apptime.GetTimeStamp("%Y-%m-%dT%H:%M") << " | " << Cli::Color::LightRed		<< "[ Error ] ";    break; }
                    case LogLevel::Warn:        { mStream << Cli::Color::Gray << apptime.GetTimeStamp("%Y-%m-%dT%H:%M") << " | " << Cli::Color::LightYellow		<< "[ Warn  ] ";    break; }
                    case LogLevel::Info:        { mStream << Cli::Color::Gray << apptime.GetTimeStamp("%Y-%m-%dT%H:%M") << " | " << Cli::Color::LightGray		<< "[ Info  ] ";    break; }
                    case LogLevel::Debug:       { mStream << Cli::Color::Gray << apptime.GetTimeStamp("%Y-%m-%dT%H:%M") << " | " << Cli::Color::LightGreen		<< "[ Debug ] ";    break; }
                    case LogLevel::Trace:       { mStream << Cli::Color::Gray << apptime.GetTimeStamp("%Y-%m-%dT%H:%M") << " | " << Cli::Color::LightMagenta    << "[ Trace ] ";    break; }

                    case LogLevel::Caption:     {
                        mCaptionActive = true;
                        mStream << Cli::Color::LightBlue << "\n▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬\n";
                        break;
                    }
                    case LogLevel::Delimiter:   {
                        mStream << Cli::Color::Yellow    << "----------------------------------------------------------------\n";
                        break;
                    }
                    
                    default:                    { mStream << Cli::Color::White;  break; }
                }
            } else {
                mSkip = true;
            }
        }

        return (*this);
    }
   
    Log &operator<<(ostream &(*T)(ostream &)) {
        std::unique_lock<mutex> lock(mSync);
        if (mSkip) return (*this);

        if (mCaptionActive) {
            mStream << "\n▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬";
            mCaptionActive = false;
        }
        
        Reset();
        mCounter++;

        mStream << T;
        return (*this);
    }

    // Accessors
    size_t GetCounter() const { return mCounter; }
    LogLevel GetLevel() const { return mLogLevel; }

    // Mutators
    void SetLevel(const LogLevel level) { mLogLevel = level; }

    // Methods
    static Log &Instance() {
        static Log instance;
        return instance;
    }
    inline void Reset() {
        mStream << Cli::Style::Reset << Cli::Color::White;
    }
    static void Test() {
        Log::Instance() << LogLevel::Caption    << "Caption" << "\n";
        Log::Instance() << LogLevel::Default    << "Default" << "\n";
        Log::Instance() << LogLevel::Delimiter;
        
        Log::Instance() << LogLevel::Fatal      << "Fatal" << "\n";
        Log::Instance() << LogLevel::Error      << "Error" << "\n";
        Log::Instance() << LogLevel::Warn       << "Warn" << "\n";
        Log::Instance() << LogLevel::Info       << "Info" << "\n";
        Log::Instance() << LogLevel::Debug      << "Debug" << "\n";
        Log::Instance() << LogLevel::Trace      << "Trace" << "\n";

        Log::Instance() << LogLevel::Delimiter;
        Log::Instance() << LogLevel::Default    << "This "
            << Cli::Background::White << Cli::Color::Black << "is "
            << Cli::Background::Default << Cli::Color::LightBlue << "a "
            << Cli::Color::LightRed     << "multi" << Cli::Color::LightGray << "-"
            << Cli::Color::LightGreen   << "color" << Cli::Color::LightCyan << "-"
            << Cli::Color::LightBlue    << "test"
            << Cli::Color::Default      << "!\n";


        char Char[] = "Char *\n";
        const char *ConstChar = "ConstChar *\n";
        //wchar_t WChar_T[] = L"WChar_T *\n";
        //const wchar_t *ConstWChar_T = L"ConstWChar_T *\n";
        std::string String = "String\n";
        //std::wstring WString = L"WString\n";

        Log::Instance() << Cli::Color::LightBlue << Char;
        Log::Instance() << Cli::Color::LightBlue << ConstChar;
        //Log::Instance() << Cli::Color::LightBlue << WChar_T;
        //Log::Instance() << Cli::Color::LightBlue << ConstWChar_T;
        Log::Instance() << Cli::Color::LightBlue << String;
        //Log::Instance() << Cli::Color::LightBlue << WString;
        Log::Instance() << Cli::Color::LightBlue << std::endl;
    }

private:
    // Properties
    LogLevel mLogLevel = LogLevel::Trace;
    ostream &mStream = std::cout;
    mutex mSync;

    // States
    size_t mCounter = 0;
    bool mCaptionActive = false;
    bool mLevelActive = false;
    bool mSkip = false;
};

// This is the preferred instance to the logging class.
Log &applog = Ultra::Log::Instance();

///
/// @brief  As good as the logging class can be, we need something for applications where performance matters. Therefore the following function templates
///         are for convenience, they will help removing unaccessary code in distribution builds.
///
template<typename ...Args> void AppLog(Args &&...args)			{ applog << LogLevel::Default   ; (applog << ... << args); applog << "\n"; }
template<typename ...Args> void AppLogTrace(Args &&...args)		{ applog << LogLevel::Trace     ; (applog << ... << args); applog << "\n"; }
template<typename ...Args> void AppLogDebug(Args &&...args)		{ applog << LogLevel::Debug     ; (applog << ... << args); applog << "\n"; }
template<typename ...Args> void AppLogInfo(Args &&...args)		{ applog << LogLevel::Info	    ; (applog << ... << args); applog << "\n"; }
template<typename ...Args> void AppLogWarning(Args &&...args)   { applog << LogLevel::Warn	    ; (applog << ... << args); applog << "\n"; }
template<typename ...Args> void AppLogError(Args &&...args)		{ applog << LogLevel::Error	    ; (applog << ... << args); applog << "\n"; }
template<typename ...Args> void AppLogFatal(Args &&...args)	    { applog << LogLevel::Fatal     ; (applog << ... << args); applog << "\n"; throw std::runtime_error(""); }

// ToDo: Not working as before in a module, maybe a default override will be needed...
#define APP_LOG(...)			AppLog			("[", __FUNCTION__, "]: ", __VA_ARGS__)
#define APP_LOG_TRACE(...)      AppLogTrace		("[", __FUNCTION__, "]: ", __VA_ARGS__)
#define APP_LOG_DEBUG(...)      AppLogDebug		("[", __FUNCTION__, "]: ", __VA_ARGS__)
#define APP_LOG_INFO(...)		AppLogInfo		("[", __FUNCTION__, "]: ", __VA_ARGS__)
#define APP_LOG_WARN(...)		AppLogWarning	("[", __FUNCTION__, "]: ", __VA_ARGS__)
#define APP_LOG_ERROR(...)		AppLogError		("[", __FUNCTION__, "]: ", __VA_ARGS__)
#define APP_LOG_FATAL(...)	    AppLogFatal	    ("[", __FUNCTION__, "]: ", __VA_ARGS__)

#ifdef APP_DEBUG_MODE
    template<typename T, typename ...Args> bool AppAssert(T *object, Args &&...args) {
        if (!object) {
            applog << Log::Critical; (applog << ... << args); applog << "\n";
            return true;
        }
        return false;
    }
    template<typename T, typename ...Args> bool AppAssert(T object, Args &&...args) {
        if (!object) {
            applog << Log::Critical; (applog << ... << args); applog << "\n";
            return true;
        }
        return false;
    }
    #define AppAssert(x, ...) if(AppAssert(x, __VA_ARGS__)) APP_DEBUGBREAK() // Workaround, at the debug break after the message.
    template<typename ...Args> void AppLogDebug(Args &&...args)		{ applog << Log::Debug		; (applog << ... << args); applog << "\n"; }
    template<typename ...Args> void AppLogTrace(Args &&...args)		{ applog << Log::Trace		; (applog << ... << args); applog << "\n"; }
    
    // Old-School: If anybody wishes preprocessor macros, we have no problem with it.
    #define APP_ASSERT(x, ...)		{ if(!(x)) { AppLogCritical("[", __FUNCTION__, "]: ", __VA_ARGS__); APP_DEBUGBREAK(); } }
#else
    // ToDo: Should we use empty functions or replace everything with nothing with the preprocesor?
    //template<typename T, typename ...Args> void AppAssert(T *object, Args &&...args) {}
    //template<typename ...Args> void AppLog(Args &&...args)			{}
    //template<typename ...Args> void AppLogInfo(Args &&...args)		{}
    //template<typename ...Args> void AppLogWarning(Args &&...args)		{}
    //template<typename ...Args> void AppLogError(Args &&...args)		{}
    //template<typename ...Args> void AppLogCritical(Args &&...args)	{}
    //template<typename ...Args> void AppLogDebug(Args &&...args)		{}
    //template<typename ...Args> void AppLogTrace(Args &&...args)		{}
    // ToDo: Should the application get killed if something goes "very" wrong, or stay up?
    //template<typename T, typename ...Args> void AppAssert(T *object, Args &&...args) {
    //	if (!object) {
    //		applog << Log::Critical; (applog << ... << args); applog << "\n";
    //		std::abort();
    //	}
    //}
    // Either works just fine...
    #define AppAssert(...);
    //#define AppLog(...);
    //#define AppLogInfo(...);
    //#define AppLogWarning(...);
    //#define AppLogError(...);
    //#define AppLogCritical(...);
    //#define AppLogDebug(...);
    //#define AppLogTrace(...);

    // Old-School
    #define APP_ASSERT(x, ...);
    //#define APP_LOG(...);
    //#define APP_LOG_INFO(...);
    //#define APP_LOG_WARN(...);	
    //#define APP_LOG_ERROR(...);	
    //#define APP_LOG_CRITICAL(...);
    //#define APP_LOG_DEBUG(...);
    //#define APP_LOG_TRACE(...);
#endif

}
