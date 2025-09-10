#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>
#include <imgui.h>

#include "editor_ui.hpp"
#include "services/locator.hpp"
#include "utility/runtime_assert.hpp"

namespace fro
{
   void EditorUI::initialise_backend()
   {
      auto const& renderer{ Locator::get<Renderer>() };
      bool succeeded{
         ImGui_ImplSDL3_InitForSDLRenderer(&renderer.assigned_native_window(), renderer.native_renderer_.get())
      };
      runtime_assert(succeeded, "failed to initialize ImGui's SDL implementation");

      succeeded = ImGui_ImplSDLRenderer3_Init(renderer.native_renderer_.get());
      runtime_assert(succeeded, "failed to initialize ImGui for SDL renderer");
   }

   void EditorUI::shutdown_backend()
   {
      ImGui_ImplSDLRenderer3_Shutdown();
      ImGui_ImplSDL3_Shutdown();
   }

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
      initialise_backend();
   }

   EditorUI::~EditorUI()
   {
      shutdown_backend();
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

   void EditorUI::begin() const
   {
      ImGui::Begin("Window");
   }

   void EditorUI::end() const
   {
      ImGui::End();
   }

   bool EditorUI::button() const
   {
      return ImGui::Button("Switch Windows");
   }

   void EditorUI::begin_frame() const
   {
      ImGui_ImplSDLRenderer3_NewFrame();
      ImGui_ImplSDL3_NewFrame();
      ImGui::NewFrame();
   }

   void EditorUI::end_frame() const
   {
      auto& render_context{ Locator::get<Renderer>() };
      Renderer::ScalingMode const scaling_mode{ render_context.scaling_mode() };
      render_context.change_scaling_mode(Renderer::ScalingMode::NONE);
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