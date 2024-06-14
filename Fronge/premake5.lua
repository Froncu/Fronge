project "Fronge"
	kind "SharedLib"

	dofile "../Premake/globalprj.lua"
	dofile "../Premake/defines.lua"

	files "Source/**"
	targetdir ("../Output/" .. outputdir)
	objdir ("../Intermediate/" .. outputdir)

	includedirs "Source"

	dofile "../External/SDL2/premake5.lua"

	defines { "FRO_BUILD_DLL", "FRO_ENGINE" }

	postbuildcommands {
		"mkdir %[../Output/Include]",
		"xcopy %[Source/*.h] %[../Output/Include] /S /Y",
		"xcopy %[Source/*.hpp] %[../Output/Include] /S /Y" }

project "*"