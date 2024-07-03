local relativepath = os.getcwd()

libdirs "lib/%{cfg.platform}"
links "SDL2_mixer"
includedirs "include"
postbuildcommands ("xcopy %[" .. relativepath .. "/lib/%{cfg.platform}/*.dll] %[%{cfg.buildtarget.directory}] /S /Y")