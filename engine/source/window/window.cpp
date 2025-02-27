#include <SDL3/SDL.h>

#include "froch.hpp"
#include "utility/assert.hpp"
#include "window.hpp"

namespace fro
{
   Window::Window(std::string_view const title, Vector2<int> const size)
      : native_window_{
         [title, size]
         {
            if (not SDL_InitSubSystem(SDL_INIT_VIDEO))
               exception("failed to initialize the video subsystem ({})", SDL_GetError());

            return SDL_CreateWindow(title.data(), size.x, size.y, SDL_WINDOW_RESIZABLE);
         }(),
         [](SDL_Window* const window)
         {
            SDL_QuitSubSystem(SDL_INIT_VIDEO);
            SDL_DestroyWindow(window);
         }
      }
   {
   }

   void Window::change_fullscreen_mode(bool const fullscreen) const
   {
      bool const succeeded{ SDL_SetWindowFullscreen(native_window_.get(), fullscreen) };
      assert(succeeded, "failed to set Window{}'s fullscreen to {} ({})",
         id(), fullscreen, SDL_GetError());
   }

   std::uint32_t Window::id() const
   {
      std::uint32_t const id{ SDL_GetWindowID(native_window_.get()) };
      assert(id, "failed to retrieve the ID of a Window titled {} ({})",
         title(), SDL_GetError());

      return id;
   }

   Vector2<int> Window::size() const
   {
      Vector2<int> size;
      bool const succeeded{ SDL_GetWindowSize(native_window_.get(), &size.x, &size.y) };
      assert(succeeded, "failed to retrieve Window{}'s size ({})",
         id(), SDL_GetError());

      return size;
   }

   std::string_view Window::title() const
   {
      return SDL_GetWindowTitle(native_window_.get());
   }
}