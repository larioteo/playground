workspace_files {
    ["Build"] = {
        "Solution.lua",
        "3rd-Party/Build/Modules.lua",
        "3rd-Party/3rd-Party.lua",
        "Source/Projects.lua",
        "Source/App/App.lua",
        "Source/Library/Library.lua",
    },
    ["Config"] = {
		".clang-format",
        ".editorconfig",
        ".gitignore",
        "Source/Settings.h",
        "Source/Style.h",
    },
    ["Manual"] = {
        "License.md",
        "ReadMe.md"
    }
}

include "App/App.lua"
include "Library/Library.lua"
