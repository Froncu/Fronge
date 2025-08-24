#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <imgui.h>

#include "core.hpp"
#include "utility/unique_pointer.hpp"

namespace fro
{
   class Application
   {
      public:
         FRO_API Application();
         FRO_API Application(Application const&);
         FRO_API Application(Application&&) noexcept;

         FRO_API virtual ~Application();

         Application& operator=(Application const&) = delete;
         Application& operator=(Application&&) = delete;

         virtual void run() = 0;

      protected:
         UniquePointer<ImGuiContext> const imgui_context_{ ImGui::CreateContext(), ImGui::DestroyContext };
   };
}

#endif