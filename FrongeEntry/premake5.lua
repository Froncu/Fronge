project "FrongeEntry"
	kind "StaticLib"

	dofile "../Premake/globalprj.lua"
	dofile "../Premake/defines.lua"

	--defines "FRO_ENGINE"

	files "Source/**"
	includedirs "../Fronge/Source"
	targetdir ("../Output/" .. outputdir)
	objdir ("../Intermediate/" .. outputdir .. "/%{prj.name}")

	links "SDL2main"

	dofile "../External/SDL2/premake5.lua"

project "*"