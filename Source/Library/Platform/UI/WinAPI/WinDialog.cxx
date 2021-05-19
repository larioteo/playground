module;

#undef APIENTRY
#define NOMINMAX
//#define VC_EXTRALEAN
//#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "Ultra/Core.h"

module Platform.UI.WinAPI.Popup;

namespace Ultra {

string Popup::FileDialog(const char *filter) const { return {}; }

#ifdef APP_DISABLED_CODE
string WinDialog::OpenFile(const char *filter) const {
    OPENFILENAMEA dialog;       // common dialog box structure
    CHAR szFile[260] = { 0 };   // if using TCHAR macros

    // Initialize OPENFILENAME
    ZeroMemory(&dialog, sizeof(dialog));
    dialog.lStructSize = sizeof(dialog);
    dialog.hwndOwner = (HWND)Application::GetWindow().GetNativeWindow();
    dialog.lpstrFile = szFile;
    dialog.nMaxFile = sizeof(szFile);
    dialog.lpstrFilter = filter;
    dialog.nFilterIndex = 1;
    dialog.lpstrFileTitle = NULL;
    dialog.nMaxFileTitle = 0;
    dialog.lpstrInitialDir = NULL;
    dialog.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

    if (GetOpenFileName(&dialog) == TRUE) {
        return dialog.lpstrFile;
    }
    return std::string();
}

string WinDialog::SaveFile(const char *filter) const {
    OPENFILENAMEA dialog;       // common dialog box structure
    CHAR szFile[260] = { 0 };   // if using TCHAR macros

     // Initialize OPENFILENAME
    ZeroMemory(&dialog, sizeof(dialog));
    dialog.lStructSize = sizeof(dialog);
    dialog.hwndOwner = (HWND)Application::GetWindow().GetNativeWindow();
    dialog.lpstrFile = szFile;
    dialog.nMaxFile = sizeof(szFile);
    dialog.lpstrFilter = filter;
    dialog.nFilterIndex = 1;
    dialog.lpstrFileTitle = NULL;
    dialog.nMaxFileTitle = 0;
    dialog.lpstrInitialDir = NULL;
    dialog.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

    if (GetSaveFileName(&dialog) == TRUE) {
        return dialog.lpstrFile;
    }
    return std::string();
}
#endif

}
