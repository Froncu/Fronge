language "C++"
cppdialect "C++20"
warnings "high"
systemversion "latest"
flags { "FatalWarnings", "MultiProcessorCompile" }

outputdir = "%{cfg.buildcfg} (%{cfg.platform})"

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

filter {}