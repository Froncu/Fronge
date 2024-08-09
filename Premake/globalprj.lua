language "C++"
cppdialect "C++20"
warnings "high"
systemversion "latest"
flags { "FatalWarnings", "MultiProcessorCompile" }

outputdir = "%{cfg.buildcfg} (%{cfg.platform})"

filter "configurations:Debug"
	symbols "full"
	optimize "off"
	runtime "debug"

filter "configurations:Release Console or configurations:Release"
	symbols "off"
	optimize "full"
	runtime "release"
	defines "NDEBUG"

filter {}