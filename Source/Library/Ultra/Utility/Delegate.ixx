module;

#include <functional>
#include <mutex>
#include <map>

#include "Ultra/Core.h"

export module Ultra.Utility.Delegate;
import Ultra.Utility.UUID;

///
/// @brief Delegate
///
export namespace Ultra {

template <typename> class Delegate;
template <typename R, typename ... Arguments>
class Delegate<R(Arguments...)> {
    using Callback = std::function<R(Arguments ...)>;
    using Callbacks = std::map<size_t, unique_ptr<Callback>>;

public:
    // Default
    Delegate() = default;
    Delegate(const Delegate &) = delete;
    Delegate(Delegate &&) = delete;
    ~Delegate() { mCallbacks.clear(); };

    // Methods
    void Attach(Callback &&callback) {
        std::unique_lock<std::mutex> lock(mMutex);
        auto key = callback.target_type().hash_code();
        mCallbacks.emplace(key, std::make_unique<Callback>(std::forward<Callback>(callback)));
    }
    void Detach(Callback &&callback) {
        std::unique_lock<std::mutex> lock(mMutex);
        auto key = callback.target_type().hash_code();
        auto it = mCallbacks.find(key);
        if (it != mCallbacks.end()) {
            mCallbacks.erase(it);
        }
    }
    bool Empty() const {
        return mCallbacks.empty();
    }
    void Publish(Arguments &&...arguments) {
        std::unique_lock<std::mutex> lock(mMutex);
        for (const auto &callback : mCallbacks) {
            (*callback.second)(arguments ...);
        }
    };

    // Operators
    void operator=(Delegate &) = delete;
    operator bool() {
        return Empty();
    }
    void operator()(Arguments &&...arguments) {
        Publish(std::forward<Arguments>(arguments)...);
    }
    Delegate &operator+=(Callback &&callback) {
        Attach(std::forward<Callback>(callback));
        return *this;
    }
    Delegate &operator-=(Callback &&callback) {
        Detach(std::forward<Callback>(callback));
        return *this;
    }

private:
    // Properties
    Callbacks mCallbacks = {};
    std::mutex mMutex = {};
};

template <typename T>
constexpr auto CreateDelegate() {
    auto result = [this](auto &&... args) {
        return T(args...);
    };
    return result;
};

///
/// @brief DelegateOld
///
//template <typename>
//class DelegateData;
//
//template <typename R, typename ... Arguments>
//class DelegateData<R(Arguments...)> {
//    using Signature = std::function<R(Arguments ...)>;
//
//public:
//    DelegateData() = delete;
//    DelegateData(shared_ptr<void> &&object, shared_ptr<void> &&caller):
//        mCaller(std::move(caller)),
//        mObject(std::move(object)) {
//    }
//    ~DelegateData() {
//        mObject.reset();
//        mCaller.reset();
//    }
//
//private:
//    Signature mSignature;
//    shared_ptr<void> mCaller = nullptr;
//    shared_ptr<void> mObject = nullptr;
//};

template <typename Signature>
class DelegateOld;

template <typename R, typename ... Arguments>
class DelegateOld<R(Arguments...)> {
    using Callback = std::function<R(Arguments ...)>;
    using Callbacks = vector<weak_ptr<Callback>>;
    using CallbacksNew = std::map<size_t, unique_ptr<Callback>>;

    struct ObserverBase {
        virtual ~ObserverBase() = default;

        virtual void operator()(Arguments ... args) = 0;
        virtual bool operator==(ObserverBase &other) = 0;
    };

    template <typename T>
    struct ObserverNew: ObserverBase {
        T Callback;

        template <typename S>
        ObserverNew(S &&callback):
            Callback(std::forward<S>(callback)) {
        }

        void operator()(Arguments ... args) {
            return this->Callback(std::forward<Arguments>(args)...);
        }
        bool operator==(ObserverBase &other) {
            ObserverNew<T> *observer = dynamic_cast<ObserverNew<T>*>(other);
            return observer && this->Callback == observer->Callback;
        }
    };

    std::vector<std::unique_ptr<ObserverBase>> mObservers;

public:
    // Default
    DelegateOld() = default;
    DelegateOld(DelegateOld &) = delete;
    DelegateOld(DelegateOld &&) = delete;
    ~DelegateOld() { mCallbacks.clear(); };

    // Methods
    void Attach(Callback &&callback) {
        std::unique_lock<std::mutex> lock(mMutex);

        auto key = callback.target_type().hash_code();
        mCallbacksNew.emplace(key, std::make_unique<Callback>(std::forward<Callback>(callback)));

        //std::shared_ptr<Callback> sharedCallback = std::make_shared<Callback>(std::move(callback));
        //mCallbacks.push_back(sharedCallback);

        //return Observer(std::move(sharedCallback));
    }
    void Detach(Callback &&callback) {
        std::unique_lock<std::mutex> lock(mMutex);

        auto key = callback.target_type().hash_code();
        auto it = mCallbacksNew.find(key);
        if (it != mCallbacksNew.end()) {
            mCallbacksNew.erase(it);
        }

        //std::shared_ptr<Callback> sharedCallback = std::make_shared<Callback>(std::forward<Callback>(callback));
        //mCallbacks.erase(std::remove_if(mCallbacks.begin(), mCallbacks.end(), callback));

        //A: mCallbacks.erase(std::remove_if(mCallbacks.begin(), mCallbacks.end(), observer));
        //    call<T> tmp(std::forward<T>(callback));
        //    auto it = std::remove_if(this->mObservers.begin(),
        //                             this->mObservers.end(),
        //                             [&](std::unique_ptr<base>& other) {
        //        return tmp.do_cmp(other.get());
        //    });
        //    this->mObservers.erase(it, this->mObservers.end());
    }
    bool Empty() const {
        return mCallbacksNew.empty();
    }
    void Publish(Arguments &&...arguments) {
        std::unique_lock<std::mutex> lock(mMutex);

        for (const auto &callback : mCallbacksNew) {
            (*callback.second)(arguments ...);
        }

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
    };

    // Operators
    void operator=(DelegateOld &) = delete;
    operator bool() {
        return Empty();
    }
    void operator()(Arguments &&...arguments) {
        Publish(std::forward<Arguments>(arguments)...);
    }
    DelegateOld &operator+=(Callback &&callback) {
        Attach(std::forward<Callback>(callback));
        return *this;
    }
    DelegateOld &operator-=(Callback &&callback) {
        Detach(std::forward<Callback>(callback));
        return *this;
    }

private:
    // Properties
    Callbacks mCallbacks = {};
    CallbacksNew mCallbacksNew = {};
    std::mutex mMutex = {};
};

}
