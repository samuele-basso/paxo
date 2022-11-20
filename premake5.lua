workspace "paxo"
    architecture "amd64"
    startproject "editor"

    configurations {
        "Debug",
        "Release"
    }

    flags {
        "MultiProcessorCompile"
    }

    outdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
    assetdir = _WORKING_DIR

    include "runtime"
    include "editor"

    group "vendor"
        include "runtime/vendor/glfw"
        include "runtime/vendor/glad"
        include "editor/vendor/imgui"
    group ""
