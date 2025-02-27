#include "froch.hpp"

#include "Logger/Logger.hpp"
#include "Utility/Assert.hpp"
#include "WindowImpl.hpp"

#include <SDL.h>

namespace fro
{
   Window::Implementation::Implementation(std::string_view const windowTitle, Vector2<int> const size)
      : mSDLWindow{
         SDL_CreateWindow(windowTitle.data(),
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            size.x, size.y,
            SDL_WINDOW_RESIZABLE),
         SDL_DestroyWindow
      }
   {
      assert(mSDLWindow.get(), "failed to create Window ({})", SDL_GetError());
      Logger::info("created a {}x{} Window titled \"{}\" with ID {}!",
         size.x, size.y, windowTitle, getID());
   }

   SDL_Window* Window::Implementation::getSDLWindow() const
   {
      return mSDLWindow.get();
   }

   std::uint32_t Window::Implementation::getID() const
   {
      // SDL's ID's for windows start from 1, Fronge's start from 0, so we subtract 1
      return SDL_GetWindowID(mSDLWindow.get()) - 1;
   }
}