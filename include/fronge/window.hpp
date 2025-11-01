#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "fronge/api.hpp"
#include "fronge/event_dispatcher.hpp"
#include "fronge/id.hpp"
#include "fronge/locator.hpp"
#include "fronge/referenceable.hpp"
#include "fronge/system_event_dispatcher.hpp"
#include "fronge/variant_visitor.hpp"
#include "fronge/vector2.hpp"
#include "fronge/window_event.hpp"

struct SDL_Window;

namespace fro
{
   class Window final : public Referenceable
   {
      public:
         FRO_API explicit Window(std::string_view title = "Application", Vector2<int> size = { 640, 480 });
         Window(Window const&) = delete;
         Window(Window&&) = default;

         virtual ~Window() override = default;

         Window& operator=(Window const&) = delete;
         Window& operator=(Window&&) = default;

         FRO_API void change_title(std::string_view title);
         FRO_API void change_size(Vector2<int> size);
         FRO_API void change_position(Vector2<int> position);
         FRO_API void center();
         FRO_API void change_fullscreen_mode(bool fullscreen);
         FRO_API void change_resizability(bool resizable);
         FRO_API void change_visibility(bool show);

         FRO_API [[nodiscard]] ID::InternalValue id() const;
         FRO_API [[nodiscard]] std::string_view title() const;
         FRO_API [[nodiscard]] Vector2<int> size() const;
         FRO_API [[nodiscard]] Vector2<int> position() const;
         FRO_API [[nodiscard]] bool fullscreen() const;
         FRO_API [[nodiscard]] bool resizable() const;
         FRO_API [[nodiscard]] bool visible() const;

         FRO_API [[nodiscard]] SDL_Window& native_window() const;

         EventDispatcher<> close_event{};
         EventListener<WindowEvent const> on_window_event
         {
            VariantVisitor
            {
               [smart_this = Reference{ this }](WindowCloseEvent const& event)
               {
                  if (smart_this->id() not_eq event.id)
                     return false;

                  smart_this->close_event.notify();
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