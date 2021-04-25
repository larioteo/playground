module;

#include "../Core.h"
//#include "Omnia/Config.h"
//#include "Omnia/GFX/Context.h"
//#include "Omnia/UI/Dialog.h"
//#include "Omnia/UI/Event.h"
//#include "Omnia/UI/GuiLayer.h"
//#include "Omnia/UI/Window.h"

#include <queue>
#include <future>
#include <functional>


export module Ultra.Application;

import Ultra.Layer;
import Ultra.LayerStack;
import Ultra.Log;
import Ultra.Utility.DateTime;
import Ultra.Utility.Timer;

export namespace Ultra {

using std::function;
using std::future;

// The title, resolution and graphics API can be passed as structure.
struct ApplicationProperties {
    ApplicationProperties(): Title("Omnia"), Resolution("800x600") { CalculateResolution(); }
    ApplicationProperties(string title, string resolution): Title(title), Resolution(resolution) { CalculateResolution(); }

    string Title;
    string Resolution;
    uint32_t Width;
    uint32_t Height;

private:
    void CalculateResolution() {
        std::string delimiter = "x";
        std::string width = Resolution.substr(0, Resolution.find(delimiter));
        std::string height = Resolution.substr(Resolution.find(delimiter) +1, Resolution.size());

        bool rwidth = !width.empty() && std::find_if(width.begin(), width.end(), [](unsigned char c) { return !std::isdigit(c); }) == width.end();
        bool rheight = !height.empty() && std::find_if(height.begin(), height.end(), [](unsigned char c) { return !std::isdigit(c); }) == height.end();

        if (!rwidth || !rheight) {
            Resolution = "1024x768";
            Width = 1024;
            Height = 768;
            return;
        }

        Width = std::stoi(width);
        Height = std::stoi(height);
    }
};

// This class contains the main workflow of the Omnia library, it can be used only once (as singleton).
class Application {
    // Types
    struct Statistics {
        //std::queue<float> fpsData;
        //std::queue<float> msPFData;

        double fps = {};
        double msPF = {};
    };

public:
    // Default
    static Application *pAppInstance;
    Application(const ApplicationProperties &properties = {}):
        Reloaded(false),
        Running(true),
        Paused(false) {
        // Preparation
        applog << "Application started..."s << "\n";
        applog << "... on: " << apptime.GetIsoDate() << "\n";
        applog << "... at: " << apptime.GetIsoTime() << "\n";
        pAppInstance = this;
        mProperties = properties;

        // Initialization
        applog << LogLevel::Caption << "Initialization" << "\n";

        // Load Configuration
        //mConfig = CreateReference<Config>();

        // Load Window, Context and Events
        //mWindow = Window::Create(WindowProperties(properties.Title, properties.Width, properties.Height));
        //pListener = EventListener::Create();
        AppLogDebug("[Application] ", "Created window '", properties.Title, "' with size '", properties.Width, "x", properties.Height, "'");

        // Load Core Layer
        //pCoreLayer = new GuiLayer();
        //PushOverlay(pCoreLayer);
    }
    virtual ~Application() {
        applog << "\nApplication finished ..."s << "\n";
        applog << "... on: " << apptime.GetIsoDate() << "\n";
        applog << "... at: " << apptime.GetIsoTime() << "\n";
    }
    static Application &Get() { return *pAppInstance; }

    // Accessors
    static ApplicationProperties &GetProperties() { return Get().mProperties; }
    static Statistics GetStatistics() { return Get().statistics; };

    // With this method, everything begins.
    void Run() {
        // Preparation
        //APP_PROFILE_BEGIN_SESSION("Application", "AppProfile.json");
        Timer timer = {};
        double delay = {};
        double frames = {};
        string statistics;
        string title = {};
        //string title = mWindow->GetTitle();

        // Subscribe to all events (internal)
        //auto oDispatcher = mWindow->EventCallback.Subscribe([&](bool &result, void *event) { pListener->Callback(result, event); });

        // Main Logic
        Create();
        for (Layer *layer : mLayers) layer->Create();
        applog << LogLevel::Caption << "Main Loop"s << "\n";

        while (Running) {
            // Update events and check if application is paused
            //pListener->Update();
            if (Paused) continue;
            if (Reloaded) {
                Reloaded = false;
                continue;
            }

            // Calcualte 
            Timestamp deltaTime = timer.GetDeltaTime();
            frames++;
            delay += deltaTime;
            this->statistics.msPF = deltaTime.GetMilliseconds();

            if (delay >= 1.0f) {
                this->statistics.fps = frames;
                float msPF = 1000.0f / (float)frames;

                statistics = title + " [FPS:" + std::to_string(frames) + " | msPF:" + std::to_string(msPF) + "]";
                //mWindow->SetTitle(statistics);

                frames = 0;
                delay = 0.0f;
            }

            // Update
            for (Layer *layer : mLayers) layer->Update(deltaTime);
            Update(deltaTime);
            //if (mWindow->GetState(WindowState::Alive)) {
            //pCoreLayer->Prepare();
            //for (Layer *layer : mLayers) layer->GuiUpdate();
            //pCoreLayer->Finish();
            //}
        }

        // Termination
        applog << LogLevel::Caption << "Termination" << "\n";
        for (Layer *layer : mLayers) layer->Destroy();
        Destroy();
        //APP_PROFILE_END_SESSION();
    }

    // These methods offer an easy to use applicaiton workflow.
    /// This method executes your initialization code.
    virtual void Create() {}
    /// This method executes your termination code.
    virtual void Destroy() {}
    /// This method executes your main logic code.
    virtual void Update(Timestamp deltaTime) {}

    // Danger-Zone
    /// Attention: Call this method only if you want to reload/switch the graphics context.
    static void Reload() {}

    // With this method, everything ends.
    void Exit() {
        Running = false;
    }

    // These methods offer an easy to use layer system.
    void PushLayer(Layer *layer) {
        mLayers.PushLayer(layer);
        layer->Attach();
    }
    void PushOverlay(Layer *overlay) {
        mLayers.PushOverlay(overlay);
        overlay->Attach();
    }

    // Theis method offer an easy to use event system.
    /// This method delivers you window events.
    //virtual void Event(EventData &data);

private:
    // Properties
    ApplicationProperties mProperties;
    Statistics statistics;

    // States
    bool Paused;
    bool Reloaded;
    bool Running;
    
    // Ojbects
    LayerStack mLayers;
};

Application *Application::pAppInstance = nullptr;

}

