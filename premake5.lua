workspace "OpenGL"
	architecture "x86_64"
	startproject "App"

	configurations
	{
		"Debug",
		"Release",
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "third_party"
	include "App/third_party/glad"
	include "App/third_party/GLFW"
group ""

project "App"
	location "App"
	kind "ConsoleApp"
	language "C"
	cdialect "C17"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.c",
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/third_party",
		"%{prj.name}/third_party/glad/include",
		"%{prj.name}/third_party/glfw/include",
		"%{prj.name}/third_party/cglm/include",
	}

	links
	{
		"glad",
		"GLFW",
	}

	filter "configurations:Debug"
		defines "APP_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "APP_RELEASE"
		runtime "Release"
		optimize "on"