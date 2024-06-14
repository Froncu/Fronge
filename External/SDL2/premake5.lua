includedirs "include"
libdirs "lib/%{cfg.platform}"
links "SDL2"
postbuildcommands ("xcopy %[../External/SDL2/lib/%{cfg.platform}/*.dll] %[%{cfg.buildtarget.directory}] /S /Y")