project "App"
    defines { "PROJECT_NAME=App" }
    kind "ConsoleApp"
    language "C++"
	characterset "Unicode"
    confromancemode "true"
    cdialectx "C17"
    cppdialect "C++Latest"
    cppmodules "true"
    toolset "msc"
	
	links { "Library" }

    files { "**.h", "**.cpp", "**.cppm", "**.ixx" }

    filter { "configurations:Debug" }
        defines { "_DEBUG" }
        symbols "On"

    filter { "configurations:Release" }
        defines { "NDEBUG" }
        optimize "On"
        symbols "On"
    
    filter { "configurations:Distribution" }
        defines { "NDEBUG" }
        optimize "On"
        symbols "Off"
    
    filter { }
