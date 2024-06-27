project "Fronge"
	kind "SharedLib"

	dofile "../Premake/globalprj.lua"
	dofile "../Premake/defines.lua"

	files "Source/**"
	
	-- Temporary
	removefiles "Source/OldFronge/**"

	targetdir ("../Output/" .. outputdir)
	objdir ("../Intermediate/" .. outputdir)

	pchheader "froch.hpp"
	pchsource "Source/froch.cpp"

	includedirs "Source"

	dofile "../External/SDL2/premake5.lua"
	dofile "../External/SDL2_image/premake5.lua"
	dofile "../External/SDL2_mixer/premake5.lua"
	dofile "../External/SDL2_ttf/premake5.lua"
	dofile "../External/vld/premake5.lua"

	defines { "FRO_BUILD_DLL", "FRO_ENGINE" }

	postbuildcommands {
		"mkdir %[../Output/Include]",
		"xcopy %[Source/*.h] %[../Output/Include] /S /Y",
		"xcopy %[Source/*.hpp] %[../Output/Include] /S /Y" }

project "*"