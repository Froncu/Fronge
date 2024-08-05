project "Fronge"
	kind "SharedLib"

	dofile "../Premake/globalprj.lua"
	dofile "../Premake/defines.lua"

	defines { "FRO_BUILD_DLL", "FRO_ENGINE" }
	
	files "Source/**"
	-- Temporary
	removefiles "Source/OldFronge/**"
	--
	includedirs { "Source", "../External/Box2D/include" }
	targetdir ("../Output/" .. outputdir)
	objdir ("../Intermediate/" .. outputdir .. "/%{prj.name}")

	pchheader "froch.hpp"
	pchsource "Source/froch.cpp"

	postbuildcommands {
		"mkdir %[../Output/Include]",
		"mkdir %[../Output/Include/External]",
		"xcopy %[Source/*.h] %[../Output/Include] /S /Y",
		"xcopy %[Source/*.hpp] %[../Output/Include] /S /Y",
		"xcopy %[../External/*.h] %[../Output/Include/External] /S /Y",
		"xcopy %[../External/*.hpp] %[../Output/Include/External] /S /Y" }

	dofile "../External/SDL2/premake5.lua"
	dofile "../External/SDL2_image/premake5.lua"
	dofile "../External/SDL2_mixer/premake5.lua"
	dofile "../External/SDL2_ttf/premake5.lua"
	dofile "../External/vld/premake5.lua"

	links "Box2D"

project "*"