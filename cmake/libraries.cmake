include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)

set(BUILD_SHARED_LIBS FALSE)
set(SDL_TEST OFF CACHE BOOL "Build the SDL2_test library")
set(SDL2_DISABLE_UNINSTALL ON CACHE BOOL "Disable uninstallation of SDL2")
FetchContent_Declare(SDL
   GIT_REPOSITORY https://github.com/libsdl-org/SDL
   GIT_TAG release-2.30.11
   GIT_PROGRESS TRUE
   GIT_SHALLOW TRUE)

FetchContent_Declare(SDL_image
   GIT_REPOSITORY https://github.com/libsdl-org/SDL_image
   GIT_TAG release-2.8.4
   GIT_PROGRESS TRUE
   GIT_SHALLOW TRUE)

set(SDL2TTF_VENDORED TRUE)
FetchContent_Declare(SDL_ttf
   GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf
   GIT_TAG release-2.24.0
   GIT_PROGRESS TRUE
   GIT_SHALLOW TRUE)

set(SDL2MIXER_VENDORED TRUE)
FetchContent_Declare(SDL_mixer
   GIT_REPOSITORY https://github.com/libsdl-org/SDL_mixer
   GIT_TAG release-2.8.0
   GIT_PROGRESS TRUE
   GIT_SHALLOW TRUE)

FetchContent_MakeAvailable(
   SDL
   SDL_image
   SDL_ttf
   SDL_mixer)