project "Fronge"
	kind "SharedLib"

	dofile "../Premake/globalprj.lua"
	dofile "../Premake/defines.lua"

	defines { "FRO_BUILD_DLL", "FRO_ENGINE" }
	
	files "Source/**"
	-- Temporary
	removefiles "Source/OldFronge/**"
	--
	includedirs "Source"
	targetdir ("../Output/" .. outputdir)
	objdir ("../Intermediate/" .. outputdir .. "/%{prj.name}")

	pchheader "froch.hpp"
	pchsource "Source/froch.cpp"

	postbuildcommands {
		"mkdir %[../Output/Include]",
		"xcopy %[Source/*.h] %[../Output/Include] /S /Y",
		"xcopy %[Source/*.hpp] %[../Output/Include] /S /Y" }

	dofile "../External/SDL2/premake5.lua"
	dofile "../External/SDL2_image/premake5.lua"
	dofile "../External/SDL2_mixer/premake5.lua"
	dofile "../External/SDL2_ttf/premake5.lua"

project "*"