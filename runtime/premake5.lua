project "runtime"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ( "%{wks.location}/bin/" .. outdir .. "/%{prj.name}" )
    objdir ( "%{wks.location}/bin-int/" .. outdir .. "/%{prj.name}" )

    files {
        "source/**.h",
        "source/**.cpp",
        "platform/windows/**.h",
        "platform/windows/**.cpp",
        "vendor/stb_image/src/stb_image.cpp"
    }

    includedirs {
        "source",
        "vendor/glfw/include",
        "vendor/glad/include",
        "vendor/spdlog/include",
        "vendor/stb_image/include",
        "vendor/nlohmann"
    }

    links {
        "glfw",
        "glad"
    }

    defines {
        "GLFW_INCLUDE_NONE",
        "RT_SHADER_QUAD_VS=\"" .. assetdir .. "/runtime/platform/glsl/quad.vs.glsl\"",
        "RT_SHADER_QUAD_FS=\"" .. assetdir .. "/runtime/platform/glsl/quad.fs.glsl\"",
        "RT_SHADER_CIRCLE_VS=\"" .. assetdir .. "/runtime/platform/glsl/circle.vs.glsl\"",
        "RT_SHADER_CIRCLE_FS=\"" .. assetdir .. "/runtime/platform/glsl/circle.fs.glsl\"",
        "RT_SHADER_LINE_VS=\"" .. assetdir .. "/runtime/platform/glsl/line.vs.glsl\"",
        "RT_SHADER_LINE_FS=\"" .. assetdir .. "/runtime/platform/glsl/line.fs.glsl\""
    }

    filter "configurations:Debug"
        defines { "RT_DEBUG_ENABLED" }
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        flags "LinkTimeOptimization"
        optimize "Speed"
        symbols "off"