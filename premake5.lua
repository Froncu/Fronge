if _ACTION == "clean" then
   os.rmdir ".vs"
   os.rmdir "Output"
   os.rmdir "Intermediate"

   os.remove "*.sln"
   os.remove "*.vcxproj"
   os.remove "*.filters"
   os.remove "*.user"
end

workspace "Fronge"
	include "Premake/globalwks.lua"

	project "Fronge"
		kind "SharedLib"

		dofile "Premake/globalprj.lua"
		dofile "Premake/defines.lua"

		files "Source/**.*"

		defines "FRO_BUILD_DLL"

		includedirs "Source"
		postbuildcommands {
			"mkdir %[Output/Include]",
			"xcopy %[Source/*.h] %[Output/Include] /S /Y",
			"xcopy %[Source/*.hpp] %[Output/Include] /S /Y" }

		include "External/SDL2"

	project "*"