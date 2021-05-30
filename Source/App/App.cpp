#include <Settings.h>

#include <format>
#include <functional>
#include <future>
#include <tuple>

#include <Ultra/EntryPoint.h>

import Ultra;

import Ultra.Debug.Memory;
import Ultra.Debug.Profiler;

// Application
namespace Ultra {

class App: public Application {
public:
    App(const ApplicationProperties &properties): Application(properties) {}
    void Create() override { Test(); }
    void Destroy() override {}
    void Update(Timestamp deltaTime) override {}

    bool Event(string test) {
        applog << "Event: " << test << "\n";
        return true;
    }
    const bool ConstEvent(string test) {
        applog << "ConstEvent: " << test << "\n";
        return true;
    }
    static bool StaticEvent(string test) {
        applog << "StaticEvent: " << test << "\n";
        return true;
    }

    // Tests
    void Test() {
        APP_PROFILE_BEGIN_SESSION("Playground", "results.json");
        // Preparation
        applog << "\n";
        applog.Location("Test");
        applog << "\n";

        // New-Stuff Zone

        Delegate<void()> delegateA;
        {
            auto lambdaA = []() { applog << "LambdaA()\n"; };
            auto lambdaB = []() { applog << "LambdaB()\n"; };

            delegateA.Attach(lambdaA);
            delegateA.Attach(lambdaA);
            delegateA += lambdaB;
            delegateA += lambdaB;

            auto lambdaC = [this](auto &&...args) { return Event(args...); };
            auto lambdaD = [this](auto &&...args) { return ConstEvent(args...); };
            //auto lambdaE = CreateDelegate<&App::Event>();

            Delegate<bool(string)> delegateB;
            delegateB.Attach(lambdaC);
            delegateB.Attach(lambdaD);
            delegateB.Attach(StaticEvent);
            delegateB += lambdaC;
            delegateB += lambdaD;
            delegateB += StaticEvent;

            delegateA();
            delegateB("EventData");

            delegateA -= lambdaB;
            delegateB.Detach(StaticEvent);
            delegateB -= StaticEvent;
            delegateB -= lambdaD;
            
            delegateB.Publish("EventData");
            delegateA.Publish();
        }
        delegateA();

        //~New-Stuff Zone

        // Single-Threaded
        auto durationST = TestST();
        applog << "MainThread [" << std::this_thread::get_id() << "]: ID" << std::endl;
        applog << "Single-Threaded[" << applog.GetCounter() << "]: " << durationST << "ms\n";

        // Multi-Threaded
        auto future1 = std::async(std::launch::async, TestMT);
        auto future2 = std::async(std::launch::async, TestMT);
        auto future3 = std::async(std::launch::async, TestMT);
        auto future4 = std::async(std::launch::async, TestMT);
        auto durationMT = future1.get() + future2.get() + future3.get() + future4.get();
        applog << "Multi-Threaded[" << applog.GetCounter() << "]: " << durationMT << "ms\n";
        VerifyMemoryUsage();
        string *newstring = new string();
        VerifyMemoryUsage();
        delete newstring;
        VerifyMemoryUsage();
        APP_PROFILE_END_SESSION(); 
    }

    static double TestST() {
        APP_PROFILE_FUNCTION();
        /// Preparation
        auto timer = Timer();

        /// Default
        // Settings
        applog << "Application Information\n"
            << " - Caption:     " << Solution::AppCaption << "\n"
            << " - Description: " << Solution::AppDescription << "\n"
            << " - Release:     " << Solution::AppRelease << "\n"
            << " - Version:     " << Solution::AppVersion << "\n"
            << "\n";
        ShowLibraryInfo();

        // Log
        AppAssert(true, "Assert");
        Log::Test();

        /// System
        // Cli
        Cli::Test();
        // FileSystem
        string ConfigFile = "D:/Development/Apps/Playground/Data/Config.yml";
        applog  << "ConfigFile: " << ConfigFile << "\n"
                << "Exists:     " << FileExists(ConfigFile)   << "\n"
                << "Root:       " << GetFileRoot(ConfigFile)  << "\n"
                << "Path:       " << GetFilePath(ConfigFile)  << "\n"
                << "Name:       " << GetFileName(ConfigFile)  << "\n"
                << "Extension:  " << GetFileExtension(ConfigFile)  << "\n"
        << "\n";
        auto ConfigData = ReadFile(ConfigFile);
        WriteFile("./Config.yml", ConfigData);
        auto files = SearchFiles("D:/Development/Apps/Playground/Data", "bat");

        /// Utilities
        // DateTime
        applog << apptime.GetTimeStamp() << "\n";
        applog << apptime.GetIsoDate() << std::endl;
        applog << apptime.GetIsoTime() << std::endl;

        // Enum
        //applog << "EventCategory::System: " << GetEnumType(Utility::EventCategory::System) << "\n";

        // EventData
        //Utility::IEventData data = { Utility::EventSource::Device };
        //applog << data;

        // String
        String::Test();
        UUID uuid;
        applog << uuid << std::endl;

        applog << "MainThread [" << std::this_thread::get_id() << "]: Done ST" << std::endl;
        return timer.GetDeltaTime();
    }

    static double TestMT() {
        APP_PROFILE_FUNCTION();
        auto timer = Timer();

        const auto Iterations = 1000;

        // Default
        for (int i = 0; i <= Iterations; i++) {
            applog << LogLevel::Default << "" << Cli::Style::Reset;
        }
        applog << "Thread [" << std::this_thread::get_id() << "]: Finished" << std::endl;

        return timer.GetDeltaTime();
    }
};

}

// Application Entry-Point
Ultra::Application *CreateApplication() {
    return new Ultra::App({ "Playground", "1024x768" });
}
