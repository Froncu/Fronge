project "Fronge"
	kind "SharedLib"

	dofile "../Premake/globalprj.lua"
	dofile "../Premake/defines.lua"

	files "Source/**.*"

	defines "FRO_BUILD_DLL"

	includedirs "Source"
	postbuildcommands {
		"mkdir %[Output/Include]",
		"xcopy %[Source/*.h] %[Output/Include] /S /Y",
		"xcopy %[Source/*.hpp] %[Output/Include] /S /Y" }

	dofile "../External/SDL2/premake5.lua"

project "*"