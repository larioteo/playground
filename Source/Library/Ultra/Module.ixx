module;

#include <iostream>
#include <memory>

#include <windows.h>

export module Ultra.System.Library;

///
/// @brief Load dynamic library (module)
/// @note under construction!
/// 

export namespace Ultra {

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
