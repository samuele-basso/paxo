project "editor"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ( "%{wks.location}/bin/" .. outdir .. "/%{prj.name}" )
    objdir ( "%{wks.location}/bin-int/" .. outdir .. "/%{prj.name}" )

    files {
        "source/Editor.h",
        "source/Editor.cpp",
        "source/components/**.h",
        "source/components/**.cpp",
        "source/extension/**.h",
        "source/extension/**.cpp",
        "source/interface/**.h",
        "source/interface/**.cpp",
        "vendor/imgui/backends/imgui_impl_opengl3.cpp",
        "vendor/imgui/backends/imgui_impl_glfw.cpp"
    }

    includedirs {
        "source",
        "vendor/imgui",
        "vendor/imgui/include",
        "../runtime/source",
        "../runtime/platform",
        "../runtime/vendor/nlohmann",
        "../runtime/vendor/glfw/include",
        "../runtime/vendor/glad/include",
        "../runtime/vendor/spdlog/include",
        "../runtime/vendor/stb_image/include"
    }

    links {
        "runtime",
        "imgui"
    }

    defines {
        "EDITOR_CONTENT_BROWSER_BASE_PATH=\"" .. assetdir .. "\"",
        "RT_SHADER_QUAD_VS=\"" .. assetdir .. "/runtime/platform/glsl/quad.vs.glsl\"",
        "RT_SHADER_QUAD_FS=\"" .. assetdir .. "/runtime/platform/glsl/quad.fs.glsl\"",
        "RT_SHADER_CIRCLE_VS=\"" .. assetdir .. "/runtime/platform/glsl/circle.vs.glsl\"",
        "RT_SHADER_CIRCLE_FS=\"" .. assetdir .. "/runtime/platform/glsl/circle.fs.glsl\"",
        "RT_SHADER_LINE_VS=\"" .. assetdir .. "/runtime/platform/glsl/line.vs.glsl\"",
        "RT_SHADER_LINE_FS=\"" .. assetdir .. "/runtime/platform/glsl/line.fs.glsl\"",
        "SHADER_MSAA_VS=\"" .. assetdir .. "/runtime/platform/glsl/msaa.vs.glsl\"",
        "SHADER_MSAA_FS=\"" .. assetdir .. "/runtime/platform/glsl/msaa.fs.glsl\""
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

project "sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ( "%{wks.location}/bin/" .. outdir .. "/%{prj.name}" )
    objdir ( "%{wks.location}/bin-int/" .. outdir .. "/%{prj.name}" )

    files {
        "source/Editor.h",
        "source/Editor.cpp",
        "source/components/**.h",
        "source/components/**.cpp",
        "source/extension/**.h",
        "source/extension/**.cpp",
    }

    includedirs {
        "source",
        "../runtime/source",
        "../runtime/platform",
        "../runtime/vendor/nlohmann",
        "../runtime/vendor/glfw/include",
        "../runtime/vendor/glad/include",
        "../runtime/vendor/spdlog/include",
        "../runtime/vendor/stb_image/include"
    }

    links {
        "runtime"
    }

    defines {
        "RT_SHADER_QUAD_VS=\"" .. assetdir .. "/runtime/platform/glsl/quad.vs.glsl\"",
        "RT_SHADER_QUAD_FS=\"" .. assetdir .. "/runtime/platform/glsl/quad.fs.glsl\"",
        "RT_SHADER_CIRCLE_VS=\"" .. assetdir .. "/runtime/platform/glsl/circle.vs.glsl\"",
        "RT_SHADER_CIRCLE_FS=\"" .. assetdir .. "/runtime/platform/glsl/circle.fs.glsl\"",
        "RT_SHADER_LINE_VS=\"" .. assetdir .. "/runtime/platform/glsl/line.vs.glsl\"",
        "RT_SHADER_LINE_FS=\"" .. assetdir .. "/runtime/platform/glsl/line.fs.glsl\"",
        "SHADER_MSAA_VS=\"" .. assetdir .. "/runtime/platform/glsl/msaa.vs.glsl\"",
        "SHADER_MSAA_FS=\"" .. assetdir .. "/runtime/platform/glsl/msaa.fs.glsl\"",
        "PX_STANDALONE"
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