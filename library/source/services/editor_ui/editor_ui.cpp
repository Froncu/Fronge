#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlgpu3.h>

#include "editor_ui.hpp"
#include "services/locator.hpp"
#include "services/renderer/renderer.hpp"
#include "services/system_event_dispatcher/system_event_dispatcher.hpp"
#include "services/window/window.hpp"
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
      bool succeeded{
         ImGui_ImplSDL3_InitForSDLGPU(&Locator::get<Window>().native_window())
      };
      runtime_assert(succeeded, "failed to initialize ImGui's SDL implementation");

      SDL_GPUDevice& gpu_device{ Locator::get<Renderer>().gpu_device() };
      ImGui_ImplSDLGPU3_InitInfo initialisation_info{
         .Device{ &gpu_device },
         .ColorTargetFormat{ SDL_GetGPUSwapchainTextureFormat(&gpu_device, &Locator::get<Window>().native_window()) },
         .MSAASamples{ SDL_GPU_SAMPLECOUNT_1 }
      };

      succeeded = ImGui_ImplSDLGPU3_Init(&initialisation_info);
      runtime_assert(succeeded, "failed to initialize ImGui for SDL renderer");
   }

   EditorUI::~EditorUI()
   {
      ImGui_ImplSDLGPU3_Shutdown();
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
      ImGui_ImplSDLGPU3_NewFrame();
      ImGui_ImplSDL3_NewFrame();
      ImGui::NewFrame();
   }

   void EditorUI::end_frame() const
   {
      ImGui::Render();
   }

   void EditorUI::prepare_render_data() const
   {
      ImGui_ImplSDLGPU3_PrepareDrawData(ImGui::GetDrawData(), Locator::get<Renderer>().command_buffer());
   }

   void EditorUI::render() const
   {
      auto const& renderer{ Locator::get<Renderer>() };
      ImGui_ImplSDLGPU3_RenderDrawData(ImGui::GetDrawData(), renderer.command_buffer(), renderer.render_pass());

      if ((ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) == ImGuiConfigFlags_ViewportsEnable)
      {
         ImGui::UpdatePlatformWindows();
         ImGui::RenderPlatformWindowsDefault();
      }
   }
}