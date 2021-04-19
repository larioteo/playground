module;

#include <iostream>
#include <memory>

#include <windows.h>

export module App.System.Library;

export namespace app {

typedef void (*TestEXT)(void);
typedef void (*TestEXT2)(void);

void Print(auto var) {
    std::cout << var;
}

void LoadModule() {
    HINSTANCE hLibrary = LoadLibrary(L"Library.dll");
   
    if (hLibrary) {
        std::cout << "Success" << std::endl;
    } else {
        std::cout << "Failure" << std::endl;
    }

    TestEXT Test = (TestEXT)GetProcAddress(hLibrary, "Test");
    if (Test) Test();

    FreeModule(hLibrary);
}

void FreeLibrary() {
}


}
