project "ImGui"
    kind "StaticLib"
    
	dofile "../../Premake/globalprj.lua"
	dofile "../../Premake/defines.lua"

    files { "**.cpp", "**.h" }

	targetdir ("Output/" .. outputdir)
	objdir ("Intermediate/" .. outputdir)
    
	includedirs { "../ImGui", "../SDL2/include" }

project "*"