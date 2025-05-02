include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)

set(SDL_TEST_LIBRARY OFF)
set(BUILD_SHARED_LIBS OFF)
FetchContent_Declare(SDL
   GIT_REPOSITORY https://github.com/libsdl-org/SDL
   GIT_TAG release-3.2.8
   GIT_PROGRESS TRUE
   GIT_SHALLOW TRUE)

set(SDLIMAGE_AVIF OFF)
FetchContent_Declare(SDL_image
   GIT_REPOSITORY https://github.com/libsdl-org/SDL_image
   GIT_TAG release-3.2.4
   GIT_PROGRESS TRUE
   GIT_SHALLOW TRUE)

set(SDLTTF_VENDORED TRUE)
FetchContent_Declare(SDL_ttf
   GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf
   GIT_TAG release-3.2.0
   GIT_PROGRESS TRUE
   GIT_SHALLOW TRUE)

FetchContent_Declare(SDL_mixer
   GIT_REPOSITORY https://github.com/libsdl-org/SDL_mixer
   GIT_TAG 863f8c0c0a1f3d8efe9b9cdb7f474efdfac54b76 # TODO: replace with a release tag
   GIT_PROGRESS TRUE
   #[[GIT_SHALLOW TRUE]]) # TODO: enable this after changing to a release tag

FetchContent_Declare(box2d
   GIT_REPOSITORY https://github.com/erincatto/box2d
   GIT_TAG v3.1.0
   GIT_PROGRESS TRUE
   GIT_SHALLOW TRUE)

FetchContent_MakeAvailable(
   SDL
   SDL_image
   SDL_ttf
   SDL_mixer
   box2d)

if(MINGW)
   target_compile_options(harfbuzz PRIVATE -Wa,-mbig-obj)
   target_compile_options(box2d PRIVATE -Wno-strict-prototypes)
else()
   target_compile_options(box2d PRIVATE /wd4255)
endif()