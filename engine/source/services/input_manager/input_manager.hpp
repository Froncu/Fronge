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
   class InputManager final
   {
      public:
         InputManager() = default;
         InputManager(InputManager const&) = delete;
         InputManager(InputManager&&) = default;

         ~InputManager() = default;

         InputManager& operator=(InputManager const&) = delete;
         InputManager& operator=(InputManager&&) = default;

         FRO_API void assign_keyboard_mouse(UserInput const& user_input);
         FRO_API void assign_gamepad(UserInput const& user_input, Gamepad const& gamepad);

         FRO_API [[nodiscard]] UserInput const& user_input(int user_input_id);
         FRO_API [[nodiscard]] std::vector<Gamepad> const& gamepads() const;

         EventDispatcher<Gamepad const> gamepad_connected_event{};
         EventDispatcher<Gamepad const> gamepad_disconnected_event{};

         EventListener<MouseButtonEvent const> on_mouse_button_event
         {
            VariantVisitor
            {
               [this](MouseButtonDownEvent const& event)
               {
                  change_input_strength(event.button, 1.0,
                     user_input(keyboard_mouse_assigned_user_input_id_));
                  return true;
               },

               [this](MouseButtonUpEvent const& event)
               {
                  change_input_strength(event.button, 0.0,
                     user_input(keyboard_mouse_assigned_user_input_id_));
                  return true;
               }
            },
            Locator::get<SystemEventDispatcher>().mouse_button_event
         };

         EventListener<KeyEvent const> on_key_event
         {
            VariantVisitor
            {
               [this](KeyDownEvent const& event)
               {
                  change_input_strength(event.key, 1.0,
                     user_input(keyboard_mouse_assigned_user_input_id_));
                  return true;
               },

               [this](KeyUpEvent const& event)
               {
                  change_input_strength(event.key, 0.0,
                     user_input(keyboard_mouse_assigned_user_input_id_));
                  return true;
               }
            },
            Locator::get<SystemEventDispatcher>().key_event
         };

         EventListener<GamepadConnectionEvent const> on_gamepad_connection_event
         {
            VariantVisitor
            {
               [this](GamepadConnectedEvent const& event)
               {
                  gamepad_connected_event.notify(gamepads_.emplace_back(event.id));
                  return true;
               },

               [this](GamepadDisconnectedEvent const& event)
               {
                  std::erase_if(gamepads_,
                     [&event](Gamepad const& gamepad)
                     {
                        return gamepad_id(gamepad) == event.id;
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
               [this](GamepadButtonDownEvent const& event)
               {
                  change_input_strength(event.button, 1.0,
                     user_input(gamepad_user_input_id(event.id)));
                  return true;
               },

               [this](GamepadButtonUpEvent const& event)
               {
                  change_input_strength(event.button, 0.0,
                     user_input(gamepad_user_input_id(event.id)));
                  return true;
               },

               [this](GamepadAxisEvent const& event)
               {
                  change_input_strength(event.axis, event.value,
                     user_input(gamepad_user_input_id(event.id)));
                  return true;
               }
            },
            Locator::get<SystemEventDispatcher>().gamepad_input_event
         };

      private:
         static void gamepad_assign_user_input_id(Gamepad const& gamepad, int user_input_id);
         [[nodiscard]] static int gamepad_id(Gamepad const& gamepad);
         [[nodiscard]] static int gamepad_user_input_id(ID::InternalValue id);

         [[nodiscard]] static double deadzoned_strength(double strength, double deadzone);
         [[nodiscard]] static double highest_strength(std::unordered_set<Input> const& inputs,
            std::unordered_map<Input, double>& input_strengths);
         static void calculate_user_input_action_values_if(UserInput const& user_input,
            std::function<bool(std::unordered_set<Input> const&)> const& predicate);

         FRO_API void change_input_strength(Input input, double new_strength, UserInput const& user_input);

         std::unordered_set<UserInput> user_inputs_{};
         std::vector<Gamepad> gamepads_{};
         int keyboard_mouse_assigned_user_input_id_{};
   };
}

#endif