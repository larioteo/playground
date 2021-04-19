#pragma once

#define APP_WINAPI
//#define APP_MODE_DEBUG

#ifdef APP_WINAPI
    #ifdef APP_MODE_DEBUG
        #define APP_TEST
    #else
        #undef APP_TEST
    #endif
#else
    #define APP_RELEASE
#endif

#include <Windows.h>

#include <iostream>
#include <string>

#include "Style.h"

import Cli;
import Module;

using app::cli;
using std::string;

namespace app { namespace core {

template<typename T> void Template(T arg) { return; }

template<typename T>
void Template2(T arg) {
    return;
}

enum class TClass {
    One     = 0,    // One
    Two     = 1,    // Two
    Three   = 2,    // Three
    Default = One,  // Default
};

class IClass {
public:
    IClass() = default;
    virtual ~IClass() = default;
};

class FClass {
public:
    FClass() = default;
    ~FClass() = default;

    void Something() { return; }
};

namespace nested {

class Class: public IClass {
    friend FClass;

public:
    Class() = default;
    Class(bool active, bool another): Active(active), Another(another) {}
    Class(bool active, bool another, bool another2, bool another3):
        Active(active),
        Another(another),
        Another2(another2),
        Another3(another3) {
    }
    virtual ~Class() = default;

    void HelloWorld() {}

protected:
    void HelloMyFriend() {}

private:
    void MySecret() {
        std::cout << "This" << 1 << "is" << "a" << "test" << "\n" << std::endl;
        int a = 0;
        int b1 = 1;
        int c2 = 2;

        string d8 = "ACE";     // Nothing special about this
        string d8000 = "ACE";  // Nothing special about this

        if (true) return;
        if (true) { auto var = 0; } else { auto var = 0; }
        if (true) {
            auto var = 0;
        } else if (false) {
            auto var = 0;
        } else {
            auto var = 0;
        }

        switch (0) {
            case 0: break;
            case 1: { break; }
            case 2: {
                auto var = 0;
                break;
            }
            default: {
                break;
            }
        }

        do { auto var = 0; } while (true);

        while (false) { auto var = 0; }
    };

    bool Active = false;
    bool Another = false;
    bool Another2 = false;
    bool Another3 = false;
};
}

}}
