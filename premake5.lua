workspace "Fronge"
	configurations { "Debug", "Release", "Distribute" }
	platforms { "x64", "x86" }

    filter "platforms:x64"
        architecture "x64"

	filter "platforms:x86"
        architecture "x86"

	filter ""

	include "Fronge"
	include "FrongeEditor"

	startproject "FrongeEditor"