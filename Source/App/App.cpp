#include <Settings.h>

#include <future>

#include <Ultra/EntryPoint.h>

import Ultra;

// Application
namespace Ultra {

class App: public Application {
public:
    App(const ApplicationProperties &properties): Application(properties) {}
    void Create() override { Test(); }
    void Destroy() override {}
    void Update(Timestamp deltaTime) override {}

    // Tests
    void Test() {
        // Preparation
        applog << "\n";
        applog.Write(LogLevel::Info);

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
    }

    static double TestST() {
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

        /// Utilities
        // DateTime
        applog << apptime.GetTimeStamp() << "\n";
        applog << apptime.GetIsoDate() << std::endl;
        applog << apptime.GetIsoTime() << std::endl;

        // Enum
        applog << "EventCategory::System: " << GetEnumType(Utility::EventCategory::System) << "\n";

        // EventData
        Utility::IEventData data = { Utility::EventSource::Device };
        applog << data;

        // String
        String::Test();
        UUID uuid;
        applog << uuid << std::endl;

        applog << "MainThread [" << std::this_thread::get_id() << "]: Done ST" << std::endl;
        return timer.GetDeltaTime();
    }

    static double TestMT() {
        auto timer = Timer();

        const auto Iterations = 10000;

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
    return new Ultra::App({ "Playground", "1280x1024" });
}
