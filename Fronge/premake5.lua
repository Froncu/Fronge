project "Fronge"
	kind "SharedLib"

	dofile "../Premake/globalprj.lua"
	dofile "../Premake/defines.lua"

	files "Source/**"
	targetdir ("../Output/" .. outputdir)
	objdir ("../Intermediate/" .. outputdir)

	pchheader "froch.hpp"
	pchsource "Source/froch.cpp"

	includedirs { "Source", "../External/ImGui" }

	dofile "../External/SDL2/premake5.lua"

	defines { "FRO_BUILD_DLL", "FRO_ENGINE" }

	links "imgui"
	postbuildcommands {
		"mkdir %[../Output/Include]",
		"xcopy %[Source/*.h] %[../Output/Include] /S /Y",
		"xcopy %[Source/*.hpp] %[../Output/Include] /S /Y" }

project "*"