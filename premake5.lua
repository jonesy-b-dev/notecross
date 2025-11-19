workspace "notecross"
configurations { "Debug", "Release" }
location "build"
startproject "notecross-cli"

-- Global settings common to all projects
filter "system:linux"
systemversion "latest"
pic "On" -- Position‑Independent Code (needed for shared libs)

filter "configurations:Debug"
defines { "DEBUG" }
symbols "On"
optimize "Off"

filter "configurations:Release"
defines { "NDEBUG" }
symbols "Off"
optimize "Speed"

--------------------------------------------------------------------
-- Helper function
function common_cpp_settings()
	language "C++"
	cppdialect "C++20"
	targetdir "%{prj.location}/bin/%{cfg.buildcfg}"
	objdir "%{prj.location}/obj/%{cfg.buildcfg}"
	includedirs { "%{prj.location}/src" }
end

--------------------------------------------------------------------
-- notecross‑daemon
project "notecross-daemon"
kind "ConsoleApp"
location "notecross-daemon"
common_cpp_settings()

files {
	"notecross-daemon/src/**.hpp",
	"notecross-daemon/src/**.cpp"
}
includedirs {
	"shared/",
	"notecross‑shared/src/include"
}

filter "system:linux"
libdirs { "../notecross‑shared/bin/%{cfg.buildcfg}" }
links { "notecross-shared" }
buildoptions { "`pkg-config --cflags libnotify`" }
linkoptions { "`pkg-config --libs libnotify`" }

--------------------------------------------------------------------
-- notecross‑cli
project "notecross-cli"
targetname "notecross"
kind "ConsoleApp"
location "notecross-cli"
common_cpp_settings()

files {
	"notecross-cli/src/**.hpp",
	"notecross-cli/src/**.cpp"
}

includedirs {
	"shared/"
}
-- Link the daemon binary so the CLI can start it (optional)
-- links { "notecross-daemon" }


--------------------------------------------------------------------
-- notecross-shared
project "notecross-shared"
targetname "notecrossShared"
kind "SharedLib"
location "notecross-shared"
common_cpp_settings()

files {
	"notecross-shared/src/**.hpp",
	"notecross-shared/src/**.cpp"
}

filter "system:linux"
buildoptions { "`pkg-config --cflags libnotify`" }
linkoptions { "`pkg-config --libs libnotify`" }
--------------------------------------------------------------------
-- Composite solution: “all” builds everything at once
newaction {
	trigger = "all",
	description = "Build both daemon and cli in one go",
	execute = function()
		os.execute("premake5 gmake2")
		os.execute("make config=debug")
		os.execute("make config=release")
	end
}
