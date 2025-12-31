workspace "OpenGLRenderer"
    architecture "x64"
    startproject "OpenGLRenderer"

    configurations
    {
        "Debug",
        "Release"
    }

project "OpenGLRenderer"
    location "OpenGLRenderer"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "off"

    targetdir ("x64/%{cfg.buildcfg}")
    objdir ("OpenGLRenderer/x64/%{cfg.buildcfg}/obj")

    files
    {
        "OpenGLRenderer/src/**.h",
        "OpenGLRenderer/src/**.hpp",
        "OpenGLRenderer/src/**.cpp",
        "libs/GLAD/src/glad.c"
    }

    includedirs
    {
        "libs/ASSIMP/include",
        "libs/GLM",
        "libs/GLAD/include",
        "libs/GLFW/include",
        "OpenGLRenderer/src"
    }

    libdirs
    {
        "libs/ASSIMP/lib",
        "libs/GLFW/lib-vc2022"
    }

    links
    {
        "glfw3",
        "opengl32"
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

        links
        {
            "assimp-vc143-mtd"
        }

        postbuildcommands
        {
            -- Copy Assimp DLL
            '{COPYFILE} "%{wks.location}libs/ASSIMP/lib/assimp-vc143-mtd.dll" "%{wks.location}x64/Debug/"',

            -- Copy shaders
            '{COPYDIR} "%{wks.location}OpenGLRenderer/shaders" "%{wks.location}x64/Debug/shaders"'
        }

    filter "configurations:Release"
        runtime "Release"
        optimize "on"
        kind "WindowedApp"

        linkoptions
        {
            "/ENTRY:mainCRTStartup"
        }

        links
        {
            "assimp-vc143-mt"
        }

        postbuildcommands
        {
            -- Copy Assimp DLL
            '{COPYFILE} "%{wks.location}libs/ASSIMP/lib/assimp-vc143-mt.dll" "%{wks.location}x64/Release/"',

            -- Copy shaders
            '{COPYDIR} "%{wks.location}OpenGLRenderer/shaders" "%{wks.location}x64/Release/shaders"'
        }

    filter {}
