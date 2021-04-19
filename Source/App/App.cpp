#include <Settings.h>
#include <Ultra/EntryPoint.h>

#include <future>

import Ultra;

// Application
namespace Ultra {

class App: public Application {
public:
    App(const ApplicationProperties &properties): Application(properties) {
    }

    void Create() override {
        Test();
    }
    
    void Destroy() override {
    }
    
    void Update(Timestamp deltaTime) override {
    }

    // Tests
    void Test() {
        // Single-Threaded
        auto durationST = TestST();

        // Multi-Threaded
        auto future1 = std::async(std::launch::async, TestMT);
        auto future2 = std::async(std::launch::async, TestMT);
        auto future3 = std::async(std::launch::async, TestMT);
        auto future4 = std::async(std::launch::async, TestMT);
        auto durationMT = future1.get() + future2.get() + future3.get() + future4.get();

        // Statistics
        applog << "\n";
        applog << "Single-Threaded[" << applog.Counter << "]: " << durationST << "ms\n";
        applog << "Multi-Threaded[" << applog.Counter << "]: " << durationMT << "ms\n";
    }

    static double TestST() {
        /// Preparation
        auto timer = Timer();

        /// Default
        // Log
        applog << LogLevel::Default << Cli::Color::White << "Hello " << Cli::Color::Blue << "Wo" << Cli::Color::Red << "rl" << Cli::Color::Yellow << "d!" << std::endl;
        AppLogInfo("Info");
        applog << Cli::Color::Red << "Haha\n";

        /// System
        // Cli
        Cli::Test();

        /// Utilities
        // DateTime
        applog << apptime.GetTimeStamp() << "\n";
        applog << apptime.GetIsoDate() << std::endl;
        applog << apptime.GetIsoTime() << std::endl;

        // Enum
        Log::Test();
        applog << "EventCategory::System: " << GetEnumType(Utility::EventCategory::System) << "\n";

        // EventData
        Utility::IEventData data = { Utility::EventSource::Device };
        applog << data;

        // String
        String::Test();
        UUID uuid;
        applog << uuid << std::endl;

        return timer.GetDeltaTime();
    }

    static double TestMT() {
        auto timer = Timer();

        const auto Iterations = 10000;

        // Default
        for (int i = 0; i <= Iterations; i++) {
            applog << LogLevel::Default << "" << Cli::Style::Reset;
        }

        return timer.GetDeltaTime();
    }
};

}

// Application Entry-Point
Ultra::Application *CreateApplication() {
    return new Ultra::App({ "Playground", "1280x1024" });
}
