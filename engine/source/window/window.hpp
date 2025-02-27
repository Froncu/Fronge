#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "core.hpp"
#include "events/observer/event_dispatcher.hpp"
#include "events/window_event.hpp"
#include "froch.hpp"
#include "maths/vector2.hpp"
#include "services/locator.hpp"
#include "services/system_event_dispatcher/system_event_dispatcher.hpp"
#include "utility/unique_pointer.hpp"
#include "utility/variant_visitor.hpp"

struct SDL_Window;

namespace fro
{
   class Window final
   {
      public:
         explicit FRO_API Window(std::string_view title = "Fronge Window", Vector2<int> size = { 640, 480 });

         Window(Window const&) = delete;
         Window(Window&&) = default;

         FRO_API ~Window() = default;

         Window& operator=(Window const&) = delete;
         Window& operator=(Window&&) = default;

         FRO_API void change_fullscreen_mode(bool fullscreen) const;
         FRO_API [[nodiscard]] std::uint32_t id() const;
         FRO_API [[nodiscard]] Vector2<int> size() const;
         FRO_API [[nodiscard]] std::string_view title() const;

         EventDispatcher<> close_event{};
         EventListener<WindowEvent const> on_window_event
         {
            VariantVisitor
            {
               [this](WindowCloseEvent const& event)
               {
                  if (id() not_eq event.id)
                     return false;

                  close_event.notify();
                  return true;
               },

               [](auto)
               {
                  return false;
               }
            },
            Locator::get<SystemEventDispatcher>().window_event
         };

      private:
         UniquePointer<SDL_Window> native_window_;
   };
}

#endif