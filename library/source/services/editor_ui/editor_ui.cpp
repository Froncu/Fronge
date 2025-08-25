#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_sdlrenderer3.h>
#include <imgui.h>

#include "editor_ui.hpp"
#include "services/locator.hpp"
#include "services/render_context/render_context.hpp"
#include "utility/runtime_assert.hpp"

namespace fro
{
   EditorUI::EditorUI()
      : imgui_context_{ ImGui::CreateContext(), ImGui::DestroyContext }
      , on_native_event_{
         [smart_this = Reference{ this }](SDL_Event const& native_event)
         {
            return ImGui_ImplSDL3_ProcessEvent(&native_event);
         },
         Locator::get<SystemEventDispatcher>().native_event
      }
   {
      ImGui::StyleColorsDark();

      auto const& render_context{ Locator::get<RenderContext>() };
      bool succeeded{
         ImGui_ImplSDL3_InitForSDLRenderer(render_context.native_window_.get(), render_context.native_renderer_.get())
      };
      runtime_assert(succeeded, "failed to initialize ImGui's SDL implementation");

      succeeded = ImGui_ImplSDLRenderer3_Init(render_context.native_renderer_.get());
      runtime_assert(succeeded, "failed to initialize ImGui for SDL renderer");
   }

   EditorUI::~EditorUI()
   {
      ImGui_ImplSDLRenderer3_Shutdown();
      ImGui_ImplSDL3_Shutdown();
   }

   bool EditorUI::captures_mouse() const
   {
      return ImGui::GetIO().WantCaptureMouse;
   }

   bool EditorUI::captures_keyboard() const
   {
      return ImGui::GetIO().WantCaptureKeyboard;
   }

   void EditorUI::show_demo_window() const
   {
      ImGui::ShowDemoWindow();
   }

   void EditorUI::begin_frame() const
   {
      ImGui_ImplSDLRenderer3_NewFrame();
      ImGui_ImplSDL3_NewFrame();
      ImGui::NewFrame();
   }

   void EditorUI::end_frame() const
   {
      auto& render_context{ Locator::get<RenderContext>() };
      RenderContext::ScalingMode const scaling_mode{ render_context.scaling_mode() };
      render_context.change_scaling_mode(RenderContext::ScalingMode::NONE);
      ImGui::Render();
      ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), render_context.native_renderer_.get());
      render_context.change_scaling_mode(scaling_mode);

      if ((ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) == ImGuiConfigFlags_ViewportsEnable)
      {
         ImGui::UpdatePlatformWindows();
         ImGui::RenderPlatformWindowsDefault();
      }
   }
}