#include <SDL3/SDL.h>

#include "froch.hpp"
#include "input_manager.hpp"
#include "utility/assert.hpp"

namespace fro
{
   void InputManager::gamepad_assign_user_input_id(Gamepad const& gamepad, int user_input_id)
   {
      bool const succeeded{ SDL_SetGamepadPlayerIndex(gamepad.native_gamepad_.get(), user_input_id) };
      assert(succeeded, "failed to assign UserInput{} to Gamepad{} ({})",
         user_input_id, gamepad_id(gamepad), SDL_GetError());
   }

   int InputManager::gamepad_id(Gamepad const& gamepad)
   {
      ID::InternalValue const id{ SDL_GetGamepadID(gamepad.native_gamepad_.get()) };
      assert(id, "failed to get the ID of a Gamepad ({})",
         SDL_GetError());

      return id;
   }

   int InputManager::gamepad_user_input_id(ID::InternalValue const id)
   {
      return SDL_GetGamepadPlayerIndexForID(id);
   }

   double InputManager::deadzoned_strength(double const strength, double const deadzone)
   {
      return strength < deadzone ? 0.0 : (strength - deadzone) / (1.0 - deadzone);
   }

   double InputManager::highest_strength(std::unordered_set<Input> const& inputs,
      std::unordered_map<Input, double>& input_strengths)
   {
      auto highest_strength{ std::numeric_limits<double>::lowest() };
      for (Input const input : inputs)
      {
         if (double const strength{ input_strengths[input] };
            strength > highest_strength)
            highest_strength = strength;
      }

      return highest_strength;
   }

   void InputManager::calculate_user_input_action_values_if(UserInput const& user_input,
      std::function<bool(std::unordered_set<Input> const&)> const& predicate)
   {
      std::unordered_map<Input, double>& input_strengths{ user_input.input_strengths_ };

      for (UserInput::InternalValueAction& internal_action : std::views::values(user_input.value_actions_))
         if (predicate(internal_action.action.inputs))
         {
            auto& [action, value_changed_event, value] = internal_action;
            auto const& [inputs, deadzone] = action;

            double const old_value{ value };

            value = deadzoned_strength(highest_strength(inputs, input_strengths), deadzone);

            if (old_value not_eq value)
               value_changed_event.notify(value);
         }

      for (UserInput::InternalAxisAction& internal_action : std::views::values(user_input.axis_actions_))
         if (predicate(internal_action.action.positive_inputs) or
            predicate(internal_action.action.negative_inputs))
         {
            auto& [action, value_changed_event, value] = internal_action;
            auto const& [positive_inputs, negative_inputs, deadzone] = action;

            double const old_value{ value };

            value =
               deadzoned_strength(highest_strength(positive_inputs, input_strengths), deadzone) -
               deadzoned_strength(highest_strength(negative_inputs, input_strengths), deadzone);

            if (old_value not_eq value)
               value_changed_event.notify(value);
         }

      for (UserInput::InternalVectorAction& internal_action : std::views::values(user_input.vector_actions_))
         if (predicate(internal_action.action.positive_x_inputs) or
            predicate(internal_action.action.negative_x_inputs) or
            predicate(internal_action.action.positive_y_inputs) or
            predicate(internal_action.action.negative_y_inputs))
         {
            auto& [action, value_changed_event, value] = internal_action;
            auto const& [positive_x_inputs, negative_x_inputs, positive_y_inputs, negative_y_inputs, deadzone] = action;

            Vector2 const old_value{ value };

            value = {
               highest_strength(positive_x_inputs, input_strengths) - highest_strength(negative_x_inputs, input_strengths),
               highest_strength(positive_y_inputs, input_strengths) - highest_strength(negative_y_inputs, input_strengths)
            };

            if (double const magnitude{ value.magnitude() }; magnitude > 1.0)
               value /= magnitude;
            else
               value *= deadzoned_strength(magnitude, deadzone);

            if (old_value not_eq value)
               value_changed_event.notify(value);
         }
   }

   void InputManager::assign_keyboard_mouse(UserInput const& user_input)
   {
      if (user_input == UserInput::GLOBAL_USER_INDEX)
         exception("attempted to assign the keyboard and mouse to the global user"
            "(the global user listens to all the input devices; it doesn't have any input devices assigned)");

         calculate_user_input_action_values_if(this->user_input(keyboard_mouse_assigned_user_input_id_),
            [](std::unordered_set<Input> const& inputs)
            {
               return std::ranges::any_of(inputs,
                  [](Input const input)
                  {
                     return std::holds_alternative<MouseButton>(input) or std::holds_alternative<Key>(input);
                  });
            });

      keyboard_mouse_assigned_user_input_id_ = user_input.id();
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

   void InputManager::change_input_strength(Input const input, double const new_strength, UserInput const& user_input)
   {
      if (user_input not_eq UserInput::GLOBAL_USER_INDEX)
         change_input_strength(input, new_strength, this->user_input(UserInput::GLOBAL_USER_INDEX));

      std::unordered_map<Input, double>& input_strengths{ user_input.input_strengths_ };

      double& input_strength{ input_strengths[input] };
      if (input_strength == new_strength)
         return;

      input_strength = new_strength;
      calculate_user_input_action_values_if(user_input,
         [input](std::unordered_set<Input> const& inputs)
         {
            return inputs.contains(input);
         });
   }
}