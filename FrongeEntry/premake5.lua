project "FrongeEntry"
	kind "StaticLib"

	dofile "../Premake/globalprj.lua"
	dofile "../Premake/defines.lua"

	files "Source/**"
	targetdir ("../Output/" .. outputdir)
	objdir ("../Intermediate/" .. outputdir)

	includedirs "../Fronge/Source"

	dofile "../External/SDL2/premake5.lua"

	links "SDL2main"

project "*"