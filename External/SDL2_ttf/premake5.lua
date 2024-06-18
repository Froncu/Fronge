includedirs "include"
libdirs "lib/%{cfg.platform}"
links "SDL2_ttf"
postbuildcommands ("xcopy %[../External/SDL2_ttf/lib/%{cfg.platform}/*.dll] %[%{cfg.buildtarget.directory}] /S /Y")