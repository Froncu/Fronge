project "FrongeEditor"
	kind "ConsoleApp"

	dofile "Premake/global.lua"
	dofile "Premake/defines.lua"

	files "Code/**"

	includedirs "../Fronge/Include"
	links "Fronge"
	postbuildcommands ("{COPYDIR} %[../Output/" .. outputdir .. "/Fronge] %[%{cfg.linktarget.directory}]")

project "*"