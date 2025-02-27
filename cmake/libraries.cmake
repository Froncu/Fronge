include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)

set(SDL_TEST_LIBRARY OFF)
set(BUILD_SHARED_LIBS OFF)
FetchContent_Declare(SDL
   GIT_REPOSITORY https://github.com/libsdl-org/SDL
   GIT_TAG release-3.2.4
   GIT_PROGRESS TRUE
   GIT_SHALLOW TRUE)

set(SDLIMAGE_AVIF OFF)
FetchContent_Declare(SDL_image
   GIT_REPOSITORY https://github.com/libsdl-org/SDL_image
   GIT_TAG release-3.2.0
   GIT_PROGRESS TRUE
   GIT_SHALLOW TRUE)

set(SDLTTF_VENDORED TRUE)
FetchContent_Declare(SDL_ttf
   GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf
   GIT_TAG prerelease-3.1.2 # TODO: replace with a release tag
   GIT_PROGRESS TRUE
   GIT_SHALLOW TRUE)

FetchContent_Declare(SDL_mixer
   GIT_REPOSITORY https://github.com/libsdl-org/SDL_mixer
   GIT_TAG 863f8c0c0a1f3d8efe9b9cdb7f474efdfac54b76 # TODO: replace with a release tag
   GIT_PROGRESS TRUE
   #[[GIT_SHALLOW TRUE]]) # TODO: enable this after changing to a release tag

FetchContent_MakeAvailable(
   SDL
   SDL_image
   SDL_ttf
   SDL_mixer)

target_compile_options(harfbuzz PRIVATE $<$<CXX_COMPILER_ID:GNU>:-Wa,-mbig-obj>)