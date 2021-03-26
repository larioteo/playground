#include <stdio.h>

#define APP_API extern "C" __declspec(dllexport)

APP_API void Test() {
    printf("A!\n");
}

APP_API void Test2() {
    printf("B!\n");
}
