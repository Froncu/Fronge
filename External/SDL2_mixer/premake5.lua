includedirs "include"
libdirs "lib/%{cfg.platform}"
links "SDL2_mixer"
postbuildcommands ("xcopy %[../External/SDL2_mixer/lib/%{cfg.platform}/*.dll] %[%{cfg.buildtarget.directory}] /S /Y")