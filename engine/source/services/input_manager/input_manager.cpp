#include "froch.hpp"
#include "input_manager.hpp"

namespace fro
{
   void InputManager::assign_keyboard_mouse(UserInput const& user_input)
   {
      if (keyboard_mouse_assigned_user_input_id_ == user_input)
         return;

      UserInput const& current_user_input{ this->user_input(keyboard_mouse_assigned_user_input_id_) };
      auto const predicate{
         [](Input const input)
         {
            return std::holds_alternative<MouseButton>(input) or std::holds_alternative<Key>(input);
         }
      };

      if (user_input == UserInput::GLOBAL_USER_ID)
         current_user_input.reset_input_strength_if(predicate);
      else if (current_user_input == UserInput::GLOBAL_USER_ID)
         user_input.copy_input_strength_if(current_user_input, predicate);
      else
         user_input.move_input_strength_if(current_user_input, predicate);

      keyboard_mouse_assigned_user_input_id_ = user_input.id();
   }

   // TODO
   void InputManager::assign_gamepad(UserInput const&, Gamepad const&)
   {
      // if (gamepad.user_input_id() == user_input)
      //    return;
      //
      // UserInput const& current_user_input{ this->user_input(gamepad.user_input_id()) };
      // auto const predicate{
      //    [](Input const input)
      //    {
      //       return std::holds_alternative<GamepadButton>(input) or std::holds_alternative<GamepadAxis>(input);
      //    }
      // };
      //
      // if (user_input == UserInput::GLOBAL_USER_ID)
      //    current_user_input.reset_input_strength_if(predicate);
      // else if (current_user_input == UserInput::GLOBAL_USER_ID)
      //    user_input.copy_input_strength_if(current_user_input, predicate);
      // else
      //    user_input.swap_input_strengths_if(current_user_input, predicate);
   }

   UserInput const& InputManager::user_input(int const user_input_id)
   {
      auto user_input{ user_inputs_.find(user_input_id) };
      if (user_input == user_inputs_.end())
         user_input = user_inputs_.insert(UserInput{ user_input_id }).first;

      return *user_input;
   }

   std::vector<Gamepad> const& InputManager::gamepads() const
   {
      return gamepads_;
   }

   void InputManager::change_input_strength(Input const input, double const strength, UserInput const& user_input)
   {
      if (user_input not_eq UserInput::GLOBAL_USER_ID)
         this->user_input(UserInput::GLOBAL_USER_ID).change_input_strength(input, strength);

      user_input.change_input_strength(input, strength);
   }
}