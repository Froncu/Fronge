#ifndef INPUT_MANAGER_HPP
#define INPUT_MANAGER_HPP

#include "core.hpp"
#include "events/input_events.hpp"
#include "events/observer/event_listener.hpp"
#include "froch.hpp"
#include "input/gamepad.hpp"
#include "input/input.hpp"
#include "input/user_input.hpp"
#include "services/locator.hpp"
#include "services/system_event_dispatcher/system_event_dispatcher.hpp"
#include "utility/variant_visitor.hpp"

namespace fro
{
   class InputManager final : public Referenceable
   {
      public:
         InputManager() = default;
         InputManager(InputManager const&) = delete;
         InputManager(InputManager&&) = default;

         virtual ~InputManager() override = default;

         InputManager& operator=(InputManager const&) = delete;
         InputManager& operator=(InputManager&&) = default;

         FRO_API void assign_keyboard_mouse(UserInput const& user_input);
         FRO_API void assign_gamepad(UserInput const& user_input, Gamepad const& gamepad);

         FRO_API UserInput const& user_input(int user_input_id);
         FRO_API [[nodiscard]] std::vector<Gamepad> const& gamepads() const;

         EventDispatcher<Gamepad const> gamepad_connected_event{};
         EventDispatcher<Gamepad const> gamepad_disconnected_event{};

         EventListener<MouseButtonEvent const> on_mouse_button_event
         {
            VariantVisitor
            {
               [smart_this = Reference{ this }](MouseButtonDownEvent const& event)
               {
                  smart_this->change_input_strength(event.button, 1.0,
                     smart_this->user_input(smart_this->keyboard_mouse_assigned_user_input_id_));
                  return true;
               },

               [smart_this = Reference{ this }](MouseButtonUpEvent const& event)
               {
                  smart_this->change_input_strength(event.button, 0.0,
                     smart_this->user_input(smart_this->keyboard_mouse_assigned_user_input_id_));
                  return true;
               }
            },
            Locator::get<SystemEventDispatcher>().mouse_button_event
         };

         EventListener<KeyEvent const> on_key_event
         {
            VariantVisitor
            {
               [smart_this = Reference{ this }](KeyDownEvent const& event)
               {
                  smart_this->change_input_strength(event.key, 1.0,
                     smart_this->user_input(smart_this->keyboard_mouse_assigned_user_input_id_));
                  return true;
               },

               [smart_this = Reference{ this }](KeyUpEvent const& event)
               {
                  smart_this->change_input_strength(event.key, 0.0,
                     smart_this->user_input(smart_this->keyboard_mouse_assigned_user_input_id_));
                  return true;
               }
            },
            Locator::get<SystemEventDispatcher>().key_event
         };

         EventListener<GamepadConnectionEvent const> on_gamepad_connection_event
         {
            VariantVisitor
            {
               [smart_this = Reference{ this }](GamepadConnectedEvent const& event)
               {
                  Gamepad gamepad{ event.id };
                  smart_this->user_input(gamepad.user_input_id());
                  smart_this->gamepad_connected_event.notify(smart_this->gamepads_.emplace_back(std::move(gamepad)));
                  return true;
               },

               [smart_this = Reference{ this }](GamepadDisconnectedEvent const& event)
               {
                  std::erase_if(smart_this->gamepads_,
                     [&event](Gamepad const& gamepad)
                     {
                        return gamepad.id() == event.id;
                     });
                  return true;
               }
            },
            Locator::get<SystemEventDispatcher>().gamepad_connection_event
         };

         EventListener<GamepadInputEvent const> on_gamepad_input_event
         {
            VariantVisitor
            {
               [smart_this = Reference{ this }](GamepadButtonDownEvent const& event)
               {
                  smart_this->change_input_strength(event.button, 1.0,
                     smart_this->user_input(Gamepad::user_input_id(event.id)));
                  return true;
               },

               [smart_this = Reference{ this }](GamepadButtonUpEvent const& event)
               {
                  smart_this->change_input_strength(event.button, 0.0,
                     smart_this->user_input(Gamepad::user_input_id(event.id)));
                  return true;
               },

               [smart_this = Reference{ this }](GamepadAxisEvent const& event)
               {
                  smart_this->change_input_strength(event.axis, event.value,
                     smart_this->user_input(Gamepad::user_input_id(event.id)));
                  return true;
               }
            },
            Locator::get<SystemEventDispatcher>().gamepad_input_event
         };

      private:
         FRO_API void change_input_strength(Input input, double strength, UserInput const& user_input);

         std::unordered_set<UserInput> user_inputs_{};
         std::vector<Gamepad> gamepads_{};
         int keyboard_mouse_assigned_user_input_id_{};
   };
}

#endif