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

		files {
		"Include/**.hpp",
		"Source/**.cpp" }

		includedirs "Include"

		defines "FRO_BUILD_DLL"

	project "*"