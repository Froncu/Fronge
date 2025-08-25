#ifndef EDITOR_UI_HPP
#define EDITOR_UI_HPP

#include "core.hpp"
#include "events/observer/event_listener.hpp"
#include "reference/referenceable.hpp"
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

         FRO_API void begin_frame() const;
         FRO_API void end_frame() const;

      private:
         UniquePointer<ImGuiContext> imgui_context_;
         EventListener<SDL_Event const> on_native_event_;
   };
}

#endif