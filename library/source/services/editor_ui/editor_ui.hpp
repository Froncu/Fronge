#ifndef EDITOR_UI_HPP
#define EDITOR_UI_HPP

#include "core.hpp"
#include "events/observer/event_listener.hpp"
#include "reference/referenceable.hpp"
#include "services/locator.hpp"
#include "services/renderer/renderer.hpp"
#include "utility/unique_pointer.hpp"

struct ImGuiContext;
union SDL_Event;

namespace fro
{
   class EditorUI final : public Referenceable
   {
      public:
         FRO_API EditorUI();
         EditorUI(EditorUI const&) = delete;
         EditorUI(EditorUI&&) = default;

         FRO_API virtual ~EditorUI() override;

         EditorUI& operator=(EditorUI const&) = delete;
         EditorUI& operator=(EditorUI&&) = default;

         FRO_API [[nodiscard]] bool captures_mouse() const;
         FRO_API [[nodiscard]] bool captures_keyboard() const;

         FRO_API void show_demo_window() const;

         FRO_API void begin() const;
         FRO_API void end() const;

         FRO_API bool button() const;

         FRO_API void begin_frame() const;
         FRO_API void end_frame() const;

      private:
         static void initialise_backend();
         static void shutdown_backend();

         UniquePointer<ImGuiContext> imgui_context_;
         EventListener<SDL_Event const> on_native_event_;
         EventListener<SDL_Window> on_renderer_window_assigned_{
            [](SDL_Window&)
            {
               shutdown_backend();
               initialise_backend();
               return true;
            },
            Locator::get<Renderer>().native_window_assigned_event
         };
   };
}

#endif