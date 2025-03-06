#include <SDL3/SDL.h>

#include "froch.hpp"
#include "utility/assert.hpp"
#include "window.hpp"

namespace fro
{
   Window::Window(std::string_view title, Vector2<int> size, std::uint64_t flags, std::optional<Vector2<int>> const& position)
      : native_window_{
         [](auto&& title, auto&& size, auto&& flags)
         {
            if (not SDL_InitSubSystem(SDL_INIT_VIDEO))
               exception("failed to initialize the video subsystem ({})", SDL_GetError());

            return SDL_CreateWindow(title.data(), size.x, size.y, flags);
         }(title, size, flags),
         [](SDL_Window* const window)
         {
            SDL_QuitSubSystem(SDL_INIT_VIDEO);
            SDL_DestroyWindow(window);
         }
      }
   {
      if (position.has_value())
         change_position(*position);
   }

   Window::Window(std::string_view const title, Vector2<int> const size)
      : Window(title, size, 0)
   {
   }

   Window::Window(Window const& other)
      : Window(other.title(), other.size(), SDL_GetWindowFlags(other.native_window_.get()), other.position())
   {
   }

   Window& Window::operator=(Window const& other)
   {
      if (this == &other)
         return *this;

      *this = Window{ other };

      return *this;
   }

   void Window::change_title(std::string_view const title) const
   {
      bool const succeeded{ SDL_SetWindowTitle(native_window_.get(), title.data()) };
      assert(succeeded, "failed to set Window{}'s title to {} ({})",
         id(), title, SDL_GetError());
   }

   void Window::change_size(Vector2<int> const size) const
   {
      bool const succeeded{ SDL_SetWindowSize(native_window_.get(), size.x, size.y) };
      assert(succeeded, "failed to set Window{}'s size to {}x{} ({})",
         id(), size.x, size.y, SDL_GetError());
   }

   void Window::change_position(Vector2<int> position) const
   {
      bool const succeeded{ SDL_SetWindowPosition(native_window_.get(), position.x, position.y) };
      assert(succeeded, "failed to set Window{}'s position to {}x{} ({})",
         id(), position.x, position.y, SDL_GetError());
   }

   void Window::center() const
   {
      change_position({ SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED });
   }

   void Window::change_fullscreen_mode(bool const fullscreen) const
   {
      bool const succeeded{ SDL_SetWindowFullscreen(native_window_.get(), fullscreen) };
      assert(succeeded, "failed to set Window{}'s fullscreen to {} ({})",
         id(), fullscreen ? "fullscreen" : "windowed", SDL_GetError());
   }

   void Window::change_resizability(bool const resizable) const
   {
      bool const succeeded{ SDL_SetWindowResizable(native_window_.get(), resizable) };
      assert(succeeded, "failed to set Window{}'s resizability to {} ({})",
         id(), resizable, SDL_GetError());
   }

   void Window::change_visibility(bool const show) const
   {
      bool const succeeded{ show ? SDL_ShowWindow(native_window_.get()) : SDL_HideWindow(native_window_.get()) };
      assert(succeeded, "failed to set Window{}'s visibility to {} ({})",
         id(), show ? "show" : "hidden", SDL_GetError());
   }

   bool Window::fullscreen() const
   {
      return SDL_GetWindowFlags(native_window_.get()) & SDL_WINDOW_FULLSCREEN;
   }

   bool Window::visible() const
   {
      return not (SDL_GetWindowFlags(native_window_.get()) & SDL_WINDOW_HIDDEN);
   }

   std::uint32_t Window::id() const
   {
      std::uint32_t const id{ SDL_GetWindowID(native_window_.get()) };
      assert(id, "failed to retrieve the ID of a Window ({})",
         SDL_GetError());

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
      assert(succeeded, "failed to retrieve Window{}'s size ({})",
         id(), SDL_GetError());

      return size;
   }

   Vector2<int> Window::position() const
   {
      Vector2<int> position;
      bool const succeeded{ SDL_GetWindowPosition(native_window_.get(), &position.x, &position.y) };
      assert(succeeded, "failed to retrieve Window{}'s position ({})",
         id(), SDL_GetError());

      return position;
   }
}