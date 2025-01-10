add_library(FrongeEntry STATIC
   Source/Entry.cpp)

get_target_property(FRONGE_INCLUDE_DIRS Fronge INCLUDE_DIRECTORIES)
target_include_directories(FrongeEntry PRIVATE
   ${FRONGE_INCLUDE_DIRS})

target_link_libraries(FrongeEntry PRIVATE
   SDL2::SDL2
   SDL2main)

set_target_properties(FrongeEntry PROPERTIES
   CXX_STANDARD 20
   CXX_STANDARD_REQUIRED TRUE)

target_compile_options(FrongeEntry PRIVATE
   /W4
   /WX)