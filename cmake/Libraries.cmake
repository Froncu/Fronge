include(fetchcontent)
set(FETCHCONTENT_QUIET FALSE)

set(BOX2D_BUILD_DOCS OFF)
set(BOX2D_BUILD_UNIT_TESTS OFF)
set(BOX2D_BUILD_TESTBED OFF)
fetchcontent_declare(Box2D
   GIT_REPOSITORY https://github.com/erincatto/box2d.git
   GIT_TAG v2.4.1
   GIT_PROGRESS TRUE
   GIT_SHALLOW TRUE)
fetchcontent_makeavailable(Box2D)

set(SDL_TEST OFF CACHE BOOL "Build the SDL2_test library")
set(SDL2_DISABLE_UNINSTALL ON CACHE BOOL "Disable uninstallation of SDL2")
fetchcontent_declare(SDL2
   GIT_REPOSITORY https://github.com/libsdl-org/SDL
   GIT_TAG release-2.30.11
   GIT_PROGRESS TRUE
   GIT_SHALLOW TRUE)
fetchcontent_makeavailable(SDL2)

fetchcontent_declare(SDL2_image
   GIT_REPOSITORY https://github.com/libsdl-org/SDL_image
   GIT_TAG release-2.8.4
   GIT_PROGRESS TRUE
   GIT_SHALLOW TRUE)
fetchcontent_makeavailable(SDL2_image)

set(SDL2TTF_VENDORED TRUE)
fetchcontent_declare(SDL2_ttf
   GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf
   GIT_TAG release-2.24.0
   GIT_PROGRESS TRUE
   GIT_SHALLOW TRUE)
fetchcontent_makeavailable(SDL2_ttf)

set(SDL2MIXER_VENDORED TRUE)
fetchcontent_declare(SDL2_mixer
   GIT_REPOSITORY https://github.com/libsdl-org/SDL_mixer
   GIT_TAG release-2.8.0
   GIT_PROGRESS TRUE
   GIT_SHALLOW TRUE)
fetchcontent_makeavailable(SDL2_mixer)