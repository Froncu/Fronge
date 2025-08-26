#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "core.hpp"
#include "events/observer/event_dispatcher.hpp"
#include "events/window_event.hpp"
#include "identifier/id.hpp"
#include "maths/vector2.hpp"
#include "reference/referenceable.hpp"
#include "services/system_event_dispatcher/system_event_dispatcher.hpp"
#include "utility/variant_visitor.hpp"

struct SDL_Window;

namespace fro
{
   class Window final : public Referenceable
   {
      public:
         FRO_API explicit Window(std::string_view title = "Application", Vector2<int> size = { 640, 480 });
         FRO_API Window(Window const& other);
         Window(Window&&) = default;

         virtual ~Window() override = default;

         FRO_API Window& operator=(Window const& other);
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
         Window(std::string_view title, Vector2<int> size, std::optional<Vector2<int>> const& position, std::uint64_t flags);

         UniquePointer<SDL_Window> native_window_;
   };
}

#endif