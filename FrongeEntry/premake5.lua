project "FrongeEntry"
	kind "StaticLib"

	dofile "../Premake/globalprj.lua"
	dofile "../Premake/defines.lua"

	files "Source/**.*"

	includedirs "../Fronge/Source"

	dofile "../External/SDL2/premake5.lua"

project "*"