filter "configurations:Debug"
	defines "FRO_DEBUG"
	defines "FRO_CONSOLE"

filter "configurations:Release Console or configurations:Release"
	defines "FRO_RELEASE"

filter "configurations:Release Console" 
	defines "FRO_CONSOLE"

filter "platforms:x64"
	defines "FRO_X64"

filter "platforms:x86"
	defines "FRO_X86"

filter {}