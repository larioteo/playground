﻿project "Library"
    defines { "PROJECT_NAME=Library" }
    kind "StaticLib"
    language "C++"
	characterset "Unicode"
    confromancemode "true"
    cdialectx "C17"
    cppdialect "C++Latest"
    cppmodules "true"
    toolset "msc"
    
    debugdir "%{wks.location}/Build/%{cfg.buildcfg}"
    files { "**.h", "**.cpp", "**.ixx" }

    filter { "configurations:Debug" }
        defines { "_DEBUG" }
        symbols "On"

    filter { "configurations:Release" }
        defines { "NDEBUG" }
        optimize "On"
        symbols "Off"
    
    filter { "configurations:Distribution" }
        defines { "NDEBUG" }
        optimize "On"
        symbols "Off"
    
    filter { }
