include(FetchContent)
set(FETCHCONTENT_QUIET OFF)

set(SDL_TEST_LIBRARY OFF)
set(BUILD_SHARED_LIBS OFF)
FetchContent_Declare(SDL
   GIT_REPOSITORY https://github.com/libsdl-org/SDL
   GIT_TAG release-3.2.20
   GIT_PROGRESS ON
   GIT_SHALLOW ON)

set(SDLIMAGE_AVIF OFF)
FetchContent_Declare(SDL_image
   GIT_REPOSITORY https://github.com/libsdl-org/SDL_image
   GIT_TAG release-3.2.4
   GIT_PROGRESS ON
   GIT_SHALLOW ON)

set(SDLTTF_VENDORED ON)
FetchContent_Declare(SDL_ttf
   GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf
   GIT_TAG release-3.2.2
   GIT_PROGRESS ON
   GIT_SHALLOW ON)

FetchContent_Declare(SDL_mixer
   GIT_REPOSITORY https://github.com/libsdl-org/SDL_mixer
   GIT_TAG 863f8c0c0a1f3d8efe9b9cdb7f474efdfac54b76 # TODO: replace with a release tag
   GIT_PROGRESS ON
   #[[GIT_SHALLOW ON]]) # TODO: enable this after changing to a release tag

FetchContent_MakeAvailable(
   SDL
   SDL_image
   SDL_ttf
   SDL_mixer)

target_compile_options(harfbuzz PRIVATE $<$<BOOL:${MINGW}>:-Wa,-mbig-obj>)