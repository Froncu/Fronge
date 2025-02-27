#ifndef SYTEM_EVENT_MANAGER_HPP
#define SYTEM_EVENT_MANAGER_HPP

#include "Core.hpp"
#include "Events/InputEvent.hpp"
#include "Events/Systems/EventListener.hpp"
#include "Events/WindowEvent.hpp"

namespace fro
{
   class SystemEventManager final
   {
      public:
         class Implementation;

         FRO_API SystemEventManager();
         SystemEventManager(SystemEventManager const&) = delete;
         SystemEventManager(SystemEventManager&&) noexcept = delete;

         FRO_API ~SystemEventManager();

         SystemEventManager& operator=(SystemEventManager const&) = delete;
         SystemEventManager& operator=(SystemEventManager&&) noexcept = delete;

         FRO_API void pollEvents();

         EventDispatcher<WindowEvent const> mWindowEvent{};
         EventDispatcher<InputEvent const> mInputEvent{};

      private:
         std::unique_ptr<Implementation> mImplementation;
   };
}

#endif