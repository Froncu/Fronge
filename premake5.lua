workspace "Fronge"
	include "Premake/globalwks.lua"

	project "Fronge"
		kind "SharedLib"

		dofile "Premake/globalprj.lua"
		dofile "Premake/defines.lua"

		files {
		"Include/**.hpp",
		"Source/**.cpp" }

		includedirs "Include"

		defines "FRO_BUILD_DLL"

	project "*"