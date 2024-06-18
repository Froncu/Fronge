includedirs "include"
libdirs "lib/%{cfg.platform}"
links "SDL2_image"
postbuildcommands ("xcopy %[../External/SDL2_image/lib/%{cfg.platform}/*.dll] %[%{cfg.buildtarget.directory}] /S /Y")