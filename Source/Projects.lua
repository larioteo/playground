workspace_files {
    ["Build"] = {
        "Solution.lua",
        "3rd-Party/Build/Modules.lua",
        "3rd-Party/3rd-Party.lua",
        "Source/Projects.lua",
        "Prepare.bat",
        "Source/App/App.lua",
        "Source/Library/Library.lua",
    },
    ["Config"] = {
        ".clang-format",
        ".editorconfig",
        ".gitignore",
        "Source/Settings.h",
    },
    ["Manual"] = {
        "License",
        "ReadMe.md",
        "Manual/Style.md",
        "Source/Style.h",
    }
}

include "App/App.lua"
include "Library/Library.lua"
