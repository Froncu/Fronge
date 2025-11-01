#include <SDL3/SDL.h>

#include "fronge/runtime_assert.hpp"
#include "fronge/window.hpp"

namespace fro
{
   Window::Window(std::string_view const title, Vector2<int> const size)
      : native_window_{
         [title, size]
         {
            SDL_Window* const native_window{ SDL_CreateWindow(title.data(), size.x, size.y, SDL_WINDOW_HIDDEN) };
            runtime_assert(native_window, std::format("failed to create an SDL window ({})", SDL_GetError()));

            return native_window;
         }(),
         SDL_DestroyWindow
      }
   {
   }

   void Window::change_title(std::string_view const title)
   {
      bool const succeeded{ SDL_SetWindowTitle(native_window_.get(), title.data()) };
      runtime_assert(succeeded, std::format("failed to set Window{}'s title to {} ({})", id(), title, SDL_GetError()));
   }

   void Window::change_size(Vector2<int> const size)
   {
      bool const succeeded{ SDL_SetWindowSize(native_window_.get(), size.x, size.y) };
      runtime_assert(succeeded,
         std::format("failed to set Window{}'s size to {}x{} ({})", id(), size.x, size.y, SDL_GetError()));
   }

   void Window::change_position(Vector2<int> position)
   {
      bool const succeeded{ SDL_SetWindowPosition(native_window_.get(), position.x, position.y) };
      runtime_assert(succeeded,
         std::format("failed to set Window{}'s position to {}x{} ({})", id(), position.x, position.y, SDL_GetError()));
   }

   void Window::center()
   {
      change_position({ SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED });
   }

   void Window::change_fullscreen_mode(bool const fullscreen)
   {
      bool const succeeded{ SDL_SetWindowFullscreen(native_window_.get(), fullscreen) };
      runtime_assert(succeeded,
         std::format("failed to set Window{}'s fullscreen to {} ({})", id(), fullscreen ? "fullscreen" : "windowed",
            SDL_GetError()));
   }

   void Window::change_resizability(bool const resizable)
   {
      bool const succeeded{ SDL_SetWindowResizable(native_window_.get(), resizable) };
      runtime_assert(succeeded,
         std::format("failed to set Window{}'s resizability to {} ({})", id(), resizable, SDL_GetError()));
   }

   void Window::change_visibility(bool const show)
   {
      bool const succeeded{ show ? SDL_ShowWindow(native_window_.get()) : SDL_HideWindow(native_window_.get()) };
      runtime_assert(succeeded,
         std::format("failed to set Window{}'s visibility to {} ({})", id(), show ? "show" : "hidden", SDL_GetError()));
   }

   ID::InternalValue Window::id() const
   {
      ID::InternalValue const id{ SDL_GetWindowID(native_window_.get()) };
      runtime_assert(id, std::format("failed to retrieve the ID of a Window ({})", SDL_GetError()));

      return id;
   }

   std::string_view Window::title() const
   {
      return SDL_GetWindowTitle(native_window_.get());
   }

   Vector2<int> Window::size() const
   {
      Vector2<int> size;
      bool const succeeded{ SDL_GetWindowSize(native_window_.get(), &size.x, &size.y) };
      runtime_assert(succeeded, std::format("failed to retrieve Window{}'s size ({})", id(), SDL_GetError()));

      return size;
   }

   Vector2<int> Window::position() const
   {
      Vector2<int> position;
      bool const succeeded{ SDL_GetWindowPosition(native_window_.get(), &position.x, &position.y) };
      runtime_assert(succeeded, std::format("failed to retrieve Window{}'s position ({})", id(), SDL_GetError()));

      return position;
   }

   bool Window::fullscreen() const
   {
      return SDL_GetWindowFlags(native_window_.get()) & SDL_WINDOW_FULLSCREEN;
   }

   bool Window::resizable() const
   {
      return SDL_GetWindowFlags(native_window_.get()) & SDL_WINDOW_RESIZABLE;
   }

   bool Window::visible() const
   {
      return not(SDL_GetWindowFlags(native_window_.get()) & SDL_WINDOW_HIDDEN);
   }

   SDL_Window& Window::native_window() const
   {
      return *native_window_;
   }
}