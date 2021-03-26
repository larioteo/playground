workspace_files {
    ["Build"] = {
        "Solution.lua",
        "3rd-Party/3rd-Party.lua",
        "Source/Projects.lua",
    },
    ["Config"] = {
		".clang-format",
        ".editorconfig",
        ".gitignore",
        "Source/Settings.h",
    },
    ["Manual"] = {
        "License.md",
        "ReadMe.md"
    }
}

include "App/App.lua"
include "Library/Library.lua"
