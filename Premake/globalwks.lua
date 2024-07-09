configurations { "Debug", "Release Console",  "Release" }
platforms { "x64", "x86" }

filter "platforms:x64"
	architecture "x64"

filter "platforms:x86"
	architecture "x86"

filter {}