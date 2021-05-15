module;

#include <filesystem>
#include <mutex>
#include <fstream>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>
#include <string_view>
#include <source_location>
#include <type_traits>

export module Ultra.Log;

import Ultra.System.Cli;
import Ultra.Utility.DateTime;
import Ultra.Utility.String;

using std::cout;
using std::mutex;
using std::ofstream;
using std::ostream;
using std::string;
using std::stringstream;

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
inline ostream &operator<<(ostream &os, T type) {
    return os << static_cast<size_t>(type);
}

///
/// @brief This logger fullfillsa all basic needs, it takes everything you throw at it (like cout), and offers easy to use features like colors, styles, etc.
///
class Log {
    // Constructors and Deconstructor
    Log() { if (mLogLevel > LogLevel::Fatal) mLogLevel = LogLevel::Fatal; }

public:
    static Log &Instance() {
        static Log instance;
        return instance;
    }
    Log(const Log &) = delete;
    Log(Log &&) noexcept = delete;
    ~Log() { Close(); };

    // Operators
    Log &operator=(const Log &) { return Instance(); }
    Log &operator=(Log &&) noexcept { return Instance(); }

    // Accessors
    const size_t &GetCounter() const { return mCounter; }
    const LogLevel &GetLevel() const { return mLogLevel; }

    // Mutators
    void SetLevel(const LogLevel level) {
        std::unique_lock<mutex> lock(mSync);
        mLogLevel = level;
    }
    void SetLogFile(const std::string_view object) {
        std::unique_lock<mutex> lock(mSync);
        LogFile = object;
    }

    // Methods
    template <typename_logmodifier T>
    Log &operator<<(T &&data) {
        std::unique_lock<mutex> lock(mSync);

        if constexpr (std::is_same_v<T, LogLevel>) {
            if (data >= mLogLevel) {
                mSkip = false;
                auto timestamp = apptime.GetTimeStamp();
                switch (data) {
                    case LogLevel::Fatal: {
                        mStream << Cli::Color::Gray << timestamp << " | " << Cli::Color::Red            << "[ Fatal ] ";
                        mFileStream                 << timestamp << " | "                               << "[ Fatal ] ";
                        break;
                    }
                    case LogLevel::Error:       {
                        mStream << Cli::Color::Gray << timestamp << " | " << Cli::Color::LightRed       << "[ Error ] ";
                        mFileStream                 << timestamp << " | "                               << "[ Error ] ";
                        break;
                    }
                    case LogLevel::Warn: {
                        mStream << Cli::Color::Gray << timestamp << " | " << Cli::Color::LightYellow    << "[ Warn  ] ";
                        mFileStream                 << timestamp << " | "                               << "[ Warn  ] ";
                        break; }
                    case LogLevel::Info: {
                        mStream << Cli::Color::Gray << timestamp << " | " << Cli::Color::LightGray	    << "[ Info  ] ";
                        mFileStream                 << timestamp << " | "                               << "[ Info  ] ";
                        break; }
                    case LogLevel::Debug: {
                        mStream << Cli::Color::Gray << timestamp << " | " << Cli::Color::LightGreen     << "[ Debug ] ";
                        mFileStream                 << timestamp << " | "                               << "[ Debug ] ";
                        break; }
                    case LogLevel::Trace: {
                        mStream << Cli::Color::Gray << timestamp << " | " << Cli::Color::LightMagenta   << "[ Trace ] ";
                        mFileStream                 << timestamp << " | "                               << "[ Trace ] ";
                        break; }

                    case LogLevel::Caption:     {
                        mCaptionActive = true;
                        mStream << Cli::Color::LightBlue    << "\n▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬\n";
                        mFileStream                         << "\n▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬\n";
                        break;
                    }
                    case LogLevel::Delimiter: {
                        mStream << Cli::Color::Yellow       << "----------------------------------------------------------------\n";
                        mFileStream                         << "----------------------------------------------------------------\n";
                        break;
                    }

                    default: {
                        mStream << Cli::Color::White;
                        break;
                    }
                }
            } else {
                mSkip = true;
            }
        }

        return (*this);
    }

    template <typename T>
    Log &operator<<(T &&data) {
        std::unique_lock<mutex> lock(mSync);
        if (mSkip) return (*this);
        if (!mFileStream.is_open()) { Open(); }

        if constexpr (is_string_v<T>) {
            if (String::EndsWith(data, "\n")) {
                if (mCaptionActive) {
                    mCaptionActive = false;
                    mStream     << "\n▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬";
                    mFileStream << "\n▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬";
                }

                mStream << Cli::Style::Reset << Cli::Color::White;
                mCounter++;
            }
        }

        mStream << std::forward<T>(data);
        mFileStream << std::forward<T>(data);
        return (*this);
    }

    Log &operator<<(ostream &(*T)(ostream &)) {
        std::unique_lock<mutex> lock(mSync);
        if (mSkip) return (*this);

        if (mCaptionActive) {
            mStream     << "\n▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬";
            mFileStream << "\n▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬";
            mCaptionActive = false;
        }

        mStream << Cli::Style::Reset << Cli::Color::White;
        mCounter++;

        mStream << T;
        mFileStream << T;
        return (*this);
    }

    // ToDo: Finish Source Location
    template <typename T>
    Log &Location(T &&data, const std::source_location &location = std::source_location::current()) {
        std::unique_lock<mutex> lock(mSync);

        mStream << " | " << location.function_name() << " | " << std::forward<T>(data);

        return (*this);
    }
    // ~ToDo

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

        // Types
        Log::Instance() << "Integers[i|l|ll|u|ul|ull]: " << 0 << 0l << 0ll << 0u << 0ul << 0ull << "\n";



        char Char[] = "Char *\n";
        const char *ConstChar = "ConstChar *\n";
        std::string String = "String\n";

        Log::Instance() << Cli::Color::LightRed << Char;
        Log::Instance() << Cli::Color::LightGreen << ConstChar;
        Log::Instance() << Cli::Color::LightBlue << String;
        Log::Instance() << "--\n";

        //wchar_t WChar_T[] = L"WChar_T *\n";
        //const wchar_t *ConstWChar_T = L"ConstWChar_T *\n";
        //std::wstring WString = L"WString\n";
        // 
        //Log::Instance() << Cli::Color::LightBlue << WChar_T;
        //Log::Instance() << Cli::Color::LightBlue << ConstWChar_T;
        //Log::Instance() << Cli::Color::LightBlue << WString;

        Log::Instance() << Cli::Color::LightBlue << std::endl;
    }

private:
    // Log-File Handling
    void Open() {
        std::filesystem::path object = LogFile;
        auto directory = object.parent_path();
        if (!directory.empty()) std::filesystem::create_directories(directory);

        mFileStream.open(object);
        mFileStream << "■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■\n";
        mFileStream << R"(  ____ ___.__   __                    _________                    __                  )" << "\n";
        mFileStream << R"( |    |   \  |_/  |_____________     /   _____/_____   ____  _____/  |_____________    )" << "\n";
        mFileStream << R"( |    |   /  |\   __\_  __ \__  \    \_____  \\____ \_/ __ \/ ___\   __\_  __ \__  \   )" << "\n";
        mFileStream << R"( |    |  /|  |_|  |  |  | \// __ \_  /        \  |_> >  ___|  \___|  |  |  | \// __ \_ )" << "\n";
        mFileStream << R"( |______/ |____/__|  |__|  (____  / /_______  /   __/ \___  >___  >__|  |__|  (____  / )" << "\n";
        mFileStream << R"(                                \/          \/|__|        \/    \/                 \/  )" << "\n";
        mFileStream << "■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■\n";
    }
    void Close() {
        mFileStream << "■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■\n";
        mFileStream.close();
    }

    // Properties
    string LogFile = "nul";
    LogLevel mLogLevel = LogLevel::Trace;
    ofstream mFileStream;
    ostream &mStream = cout;
    mutex mSync;
    mutex mSyncFile;

    // States
    size_t mCounter = 0;
    bool mCaptionActive = false;
    bool mLevelActive = false;
    bool mSkip = false;
};

// These are the global instances to the logger
Log &applog = Ultra::Log::Instance();
Log &logger = applog;

///
/// @brief  As good as a logger can be, we need something for applications where performance matters. Therefore these function templates are for convenience,
/// they will help removing unaccessary code in release and distribution builds, therefore they also override the log levels.
///
template<typename ...Args> void AppLog(Args &&...args)			{ applog << LogLevel::Default   ; (applog << ... << args); applog << "\n"; }
template<typename ...Args> void AppLogTrace(Args &&...args)		{ applog << LogLevel::Trace     ; (applog << ... << args); applog << "\n"; }
template<typename ...Args> void AppLogDebug(Args &&...args)		{ applog << LogLevel::Debug     ; (applog << ... << args); applog << "\n"; }
template<typename ...Args> void AppLogInfo(Args &&...args)		{ applog << LogLevel::Info	    ; (applog << ... << args); applog << "\n"; }
template<typename ...Args> void AppLogWarning(Args &&...args)   { applog << LogLevel::Warn	    ; (applog << ... << args); applog << "\n"; }
template<typename ...Args> void AppLogError(Args &&...args)		{ applog << LogLevel::Error	    ; (applog << ... << args); applog << "\n"; }
template<typename ...Args> void AppLogFatal(Args &&...args)	    { applog << LogLevel::Fatal     ; (applog << ... << args); applog << "\n"; }

// ToDo: Should the application get killed if something goes "very" wrong, or stay up?
#if APP_MODE_DEBUG
    template<typename T, typename ...Args> bool AppAssert(T *object, Args &&...args) {
        if (!object) {
            applog << LogLevel::Fatal; (applog << ... << args); applog << "\n";
            //std::abort(); || throw std::runtime_error("");
            return true;
        }
        return false;
    }
    template<typename T, typename ...Args> bool AppAssert(T object, Args &&...args) {
        if (!object) {
            applog << LogLevel::Fatal; (applog << ... << args); applog << "\n";
            //std::abort(); || throw std::runtime_error("");
            return true;
        }
        return false;
    }
#elif APP_MODE_RELEASE
    template<typename T, typename ...Args> void AppAssert(T *object, Args &&...args) {}
    template<typename T, typename ...Args> void AppAssert(T object, Args &&...args) {}

    template<typename ...Args> void AppLogDebug(Args &&...args)		{}
    template<typename ...Args> void AppLogTrace(Args &&...args)		{}
#elif APP_MODE_DISTRIBUTION
    template<typename T, typename ...Args> void AppAssert(T *object, Args &&...args) {}
    template<typename T, typename ...Args> void AppAssert(T object, Args &&...args) {}

    template<typename ...Args> void AppLogInfo(Args &&...args)		{}
    template<typename ...Args> void AppLogDebug(Args &&...args)		{}
    template<typename ...Args> void AppLogTrace(Args &&...args)		{}
#endif

}
