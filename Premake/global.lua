language "C++"
cppdialect "C++20"
warnings "high"
staticruntime "on"
systemversion "latest"
flags { "FatalWarnings", "MultiProcessorCompile" }

outputdir = "%{cfg.buildcfg} (%{cfg.platform})"

targetdir ("../Output/" .. outputdir .. "/%{prj.name}")
objdir ("../Intermediate/" .. outputdir .. "/%{prj.name}")

filter "configurations:Debug"
	symbols "full"
	runtime "debug"

filter "configurations:Release"
	optimize "full"
	runtime "release"

filter "configurations:Distribute"
	optimize "full"
	runtime "release"

filter {}