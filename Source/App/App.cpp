#include <Settings.h>
#include <Ultra/EntryPoint.h>

#include <future>

import App.Application;
import App.Log;
import App.System.Cli;
import App.Utility.DateTime;
import App.Utility.Enum;
import App.Utility.EventData;
import App.Utility.Random;
import App.Utility.String;
import App.Utility.Timer;
import App.Utility.UUID;

// Application
namespace app {

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
        applog << LogLevel::Default << cli::Color::White << "Hello " << cli::Color::Blue << "Wo" << cli::Color::Red << "rl" << cli::Color::Yellow << "d!" << std::endl;
        AppLogInfo("Info");
        applog << cli::Color::Red << "Haha\n";

        /// System
        // Cli
        app::cli::Test();

        /// Utilities
        // DateTime
        applog << apptime.GetTimeStamp() << "\n";
        applog << apptime.GetIsoDate() << std::endl;
        applog << apptime.GetIsoTime() << std::endl;

        // Enum
        Log::Test();
        applog << "EventCategory::System: " << GetEnumType(app::utility::EventCategory::System) << "\n";

        // EventData
        utility::IEventData data = { app::utility::EventSource::Device };
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
            applog << LogLevel::Default << "" << cli::Style::Reset;
        }

        return timer.GetDeltaTime();
    }
};

}

// Application Entry-Point
app::Application *CreateApplication() {
    return new app::App({ "Playground", "1280x1024" });
}
