project "Application"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"
    systemversion "latest"

    targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
    objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        "src/**.h",
        "src/**.cpp"
    }

    includedirs {
        "src",
        "../vendor/raylib/include",
        "../vendor/imgui",
        "../Core/src",
        "../vendor/glm",
        "../vendor/rlImGui",
        "../vendor/entt",
        "../vendor/yml-cpp/include",
    }

    libdirs {
        "../vendor/raylib/lib"
    }

    postbuildcommands { ("{COPY} ..\\vendor\\raylib\\lib\\raylib.dll ..\\bin\\" .. outputdir .. "\\Application") }

    links {
        "Core",
        "raylibdll",
        "ImGui",
    }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        runtime "Release"
        optimize "On"
