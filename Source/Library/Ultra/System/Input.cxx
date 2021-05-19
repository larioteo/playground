module;

#include "Ultra/Core.h"

module Ultra.System.Input;

#if defined(APP_PLATFORM_WINDOWS)
    import Platform.System.Input;
#endif

//namespace Ultra {
//
//Scope<Input> Input::Instance = Input::Create();
//
//Scope<Input> Input::Create() {
//    #if defined(APP_PLATFORM_WINDOWS)
//    return CreateScope<WinInput>();
//    #endif
//    return nullptr;
//}
//
//bool Input::GetKeyState(KeyCode code) {
//    return Instance->GetKeyStatePlatform(code);
//}
//
//bool Input::GetMouseButtonState(MouseButton button) {
//    return Instance->GetMouseButtonStatePlatform(button);
//}
//
//std::pair<float, float> Input::GetMousePosition() {
//    return Instance->GetMousePositionPlatform();
//}
//
//}
