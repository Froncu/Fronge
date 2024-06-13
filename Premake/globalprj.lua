language "C++"
cppdialect "C++20"
warnings "high"
staticruntime "on"
systemversion "latest"
flags { "FatalWarnings", "MultiProcessorCompile" }

outputdir = "%{cfg.buildcfg} (%{cfg.platform})"

targetdir ("../%{prj.location}/Output/" .. outputdir)
objdir ("../%{prj.location}/Intermediate/" .. outputdir)

filter "configurations:Debug"
	symbols "full"
	runtime "debug"

filter "configurations:Release"
	optimize "full"
	runtime "release"

filter "configurations:Distribute"
	optimize "full"
	symbols "off"
	runtime "release"

filter ""