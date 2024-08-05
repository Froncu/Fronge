project "Box2D"
	kind "StaticLib"

	dofile "../../Premake/globalprj.lua"

	files "src/**"
	includedirs "src"
	includedirs "include"
	targetdir ("../Output/" .. outputdir)
	objdir ("../Intermediate/" .. outputdir .. "/%{prj.name}")