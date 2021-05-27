module;

#include <algorithm>
#include <functional>
#include <mutex>
#include <map>

#include "Ultra/Core.h"

export module Ultra.Utility.Delegate;
import Ultra.Utility.UUID;
///
/// @brief Delegate
///
export namespace Ultra { namespace Utility {


// Function Holder
class Observer {
public:
    // Default
    Observer() = default;
    Observer(shared_ptr<void> &&object): mObject(std::move(object)) {};
    ~Observer() { Destroy(); }

    // Methods
    void Destroy() { mObject.reset(); };

private:
    shared_ptr<void> mObject = nullptr;
};


template <typename Signature>
class Delegate;

template <typename R, typename ... Arguments>
class Delegate<R(Arguments...)> {
    using Callback = std::function<R(Arguments...)>;
    using Callbacks = vector<weak_ptr<Callback>>;
    using CallbacksNew = std::map<size_t, unique_ptr<Callback>>;

    struct ObserverBase {
        virtual ~ObserverBase() = default;

        virtual bool operator==(ObserverBase &other) = 0;
        virtual void operator()(Arguments ... args) = 0;
    };

    template <typename T>
    struct ObserverNew: ObserverBase {
        T Callback;

        template <typename S>
        ObserverNew(S &&callback):
            Callback(std::forward<S>(callback)) {
        }

        bool operator==(ObserverBase &other) {
            ObserverNew<T> *observer = dynamic_cast<ObserverNew<T>*>(other);
            return observer && this->Callback == observer->Callback;
        }
        void operator()(Arguments ... args) {
            return this->Callback(std::forward<Arguments>(args)...);
        }
    };

    std::vector<std::unique_ptr<ObserverBase>> mObservers;

public:
    // Default
    Delegate() = default;
    Delegate(Delegate &) = delete;
    Delegate(Delegate &&) = delete;
    ~Delegate() { mCallbacks.clear(); };

    // Methods
    void Attach(Callback &&callback) {
        std::unique_lock<std::mutex> lock(mMutex);

        auto result = callback.target_type().hash_code();
        mCallbacksNew.emplace(result, std::make_unique<Callback>(callback));


        //std::shared_ptr<Callback> sharedCallback = std::make_shared<Callback>(std::move(callback));
        //mCallbacks.push_back(sharedCallback);

        //return Observer(std::move(sharedCallback));
    }
    void Detach(Callback &&callback) {
        std::unique_lock<std::mutex> lock(mMutex);

        auto result = callback.target_type().hash_code();
        auto it = mCallbacksNew.find(result);
        if (it != mCallbacksNew.end()) {
            mCallbacksNew.erase(it);
        }

        //std::shared_ptr<Callback> sharedCallback = std::make_shared<Callback>(std::move(callback));

        //mCallbacks.erase(std::remove_if(mCallbacks.begin(), mCallbacks.end(), observer));
    }
    //void Detach(Observer observer) {
    //    std::unique_lock<std::mutex> lock(mMutex);

    //    mCallbacks.erase(std::remove_if(mCallbacks.begin(), mCallbacks.end(), observer));
    //}
    bool Empty() const {
        return mCallbacksNew.empty();
    }
    void Publish(Arguments &&...arguments) {
        std::unique_lock<std::mutex> lock(mMutex);

        //std::vector<std::shared_ptr<Callback>> callbacks;
        //std::vector<std::weak_ptr<Callback>> validCallbacks;

        //{
        //    validCallbacks.reserve(mCallbacks.size());
        //    for (auto &&callback : mCallbacks) {
        //        if (auto shared = callback.lock()) {
        //            callbacks.push_back(std::move(shared));
        //            validCallbacks.push_back(std::move(callback));
        //        }
        //    }
        //    mCallbacks = std::move(validCallbacks);
        //}

        for (const auto& callback : mCallbacksNew) {
            (*callback.second)(arguments ...);
        }
    };

    // Operators
    void operator=(Delegate &) = delete;
    operator bool() {
        return Empty();
    }
    Delegate &operator+=(Callback &&callback) {
        Attach(std::forward<Callback>(callback));

        //this->mObservers.emplace_back(new ObserverNew<Callback>(std::forward<Callback>(callback)));
        return *this;
    }
    Delegate &operator-=(Callback &&callback) {
        Detach(std::forward<Callback>(callback));

        //std::shared_ptr<Callback> sharedCallback = std::make_shared<Callback>(std::forward<Callback>(callback));
        //mCallbacks.erase(std::remove_if(mCallbacks.begin(), mCallbacks.end(), callback));

        //    call<T> tmp(std::forward<T>(callback));
        //    auto it = std::remove_if(this->mObservers.begin(),
        //                             this->mObservers.end(),
        //                             [&](std::unique_ptr<base>& other) {
        //        return tmp.do_cmp(other.get());
        //    });
        //    this->mObservers.erase(it, this->mObservers.end());
        return *this;
    }
    void operator()(Arguments &&...arguments) {
        Publish(std::forward<Arguments>(arguments)...);
        //    for (auto& callback: this->d_callbacks) {
        //        callback->do_call(args...);
        //    }
    }

private:
    // Properties
    Callbacks mCallbacks = {};
    CallbacksNew mCallbacksNew = {};
    std::mutex mMutex = {};
};

}}
