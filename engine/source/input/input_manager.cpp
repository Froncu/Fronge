#include "froch.hpp"
#include "input_manager.hpp"

namespace fro
{
   void InputManager::process_input_continous()
   {
      for (auto& [input, info] : inputs_)
         info.relative_strength = 0.0;

      for (auto& [input, info] : actions_)
         info.relative_strength = 0.0;
   }

   void InputManager::bind_action_to_input(std::string const& action_name, Input const& input)
   {
      actions_[action_name].bound_inputs.insert(input);
   }

   void InputManager::change_action_deadzone(std::string const& action_name, double const deadzone)
   {
      actions_[action_name].deadzone = deadzone;
   }

   double InputManager::input_strength(Input const& input)
   {
      return inputs_[input].absolute_strength;
   }

   double InputManager::action_strength(std::string const& action_name)
   {
      return actions_[action_name].absolute_strength;
   }

   double InputManager::action_strength_axis_1d(std::string const& positive_action_name,
      std::string const& negative_action_name)
   {
      return action_strength(positive_action_name) - action_strength(negative_action_name);
   }

   Vector2<double> InputManager::action_strength_axis_2d(
      std::string const& positive_action_name_x,
      std::string const& negative_action_name_x,
      std::string const& positive_action_name_y,
      std::string const& negative_action_name_y)
   {
      Vector2 action_axis_2d{
         actions_[positive_action_name_x].raw_strength - actions_[negative_action_name_x].raw_strength,
         actions_[positive_action_name_y].raw_strength - actions_[negative_action_name_y].raw_strength
      };

      double const action_axis_2d_magnitude{ action_axis_2d.magnitude() };

      if (action_axis_2d_magnitude == 0.0)
         return action_axis_2d;

      action_axis_2d /= action_axis_2d_magnitude;

      if (action_axis_2d_magnitude > 1.0)
         return action_axis_2d;

      std::array const action_deadzones{
         actions_[positive_action_name_x].deadzone,
         actions_[negative_action_name_x].deadzone,
         actions_[positive_action_name_y].deadzone,
         actions_[negative_action_name_y].deadzone
      };

      auto const avarage_deadzone_magnitude{
         std::reduce(action_deadzones.begin(), action_deadzones.end()) / action_deadzones.size()
      };

      return action_axis_2d * deadzoned_strength(action_axis_2d_magnitude, avarage_deadzone_magnitude);
   }

   bool InputManager::input_just_activated(Input const& input)
   {
      auto const& [absoluteStrength, relativeStrength]{ inputs_[input] };

      return is_just_activated(absoluteStrength, relativeStrength);
   }

   bool InputManager::action_just_activated(std::string const& action_name)
   {
      auto const& [boundInputs, rawStrength, absoluteStrength, relativeStrength, deadzone]{ actions_[action_name] };

      return is_just_activated(absoluteStrength, relativeStrength);
   }

   bool InputManager::input_just_deactivated(Input const& input)
   {
      auto const& [absoluteStrength, relativeStrength]{ inputs_[input] };

      return is_just_deactivated(absoluteStrength, relativeStrength);
   }

   bool InputManager::action_just_deactivated(std::string const& action_name)
   {
      auto const& [boundInputs, rawStrength, absoluteStrength, relativeStrength, deadzone]{ actions_[action_name] };

      return is_just_deactivated(absoluteStrength, relativeStrength);
   }

   void InputManager::set_input_strength(Input const& input, double const new_strength)
   {
      auto&& [absolute_strength, relative_strength]{ inputs_[input] };

      relative_strength = new_strength - absolute_strength;
      absolute_strength = new_strength;

      if (auto&& [is_active, did_just_change]{ strength_info(absolute_strength, relative_strength) };
         did_just_change)
      {
         if (is_active)
            input_activated_event.notify(input, absolute_strength);
         else
            input_deactivated_event.notify(input, absolute_strength);
      }

      for (auto&& [action_name, action_info] : actions_)
         if (action_info.bound_inputs.contains(input))
         {
            double largest_bound_strength{};

            if (absolute_strength > action_info.raw_strength)
               largest_bound_strength = absolute_strength;
            else
               for (Input const& bound_input : action_info.bound_inputs)
               {
                  largest_bound_strength = std::max(input_strength(bound_input), largest_bound_strength);
                  if (largest_bound_strength == 1.0)
                     break;
               };

            action_info.raw_strength = largest_bound_strength;

            double const strength{ deadzoned_strength(action_info.raw_strength, action_info.deadzone) };
            action_info.relative_strength = strength - action_info.absolute_strength;
            action_info.absolute_strength = strength;

            if (auto const [isActive, didJustChange]{
               strength_info(action_info.absolute_strength, action_info.relative_strength)
            }; didJustChange)
            {
               if (isActive)
                  action_activated_event.notify(action_name, absolute_strength);
               else
                  action_deactivated_event.notify(action_name, absolute_strength);
            }
         }
   }

   double InputManager::deadzoned_strength(double const strength, double const deadzone)
   {
      return strength < deadzone ? 0.0 : (strength - deadzone) / (1.0 - deadzone);
   }

   bool InputManager::is_just_activated(double const absolute_strength, double const relative_strength)
   {
      auto const [is_active, did_just_change]{ strength_info(absolute_strength, relative_strength) };
      return is_active and did_just_change;
   }

   bool InputManager::is_just_deactivated(double const absolute_strength, double const relative_strength)
   {
      auto const [is_active, did_just_change]{ strength_info(absolute_strength, relative_strength) };
      return not is_active and did_just_change;
   }

   InputManager::StrengthInfo InputManager::strength_info(double const absolute_strength, double const relative_strength)
   {
      bool const is_active{ absolute_strength > 0.0 };
      double const previous_absolute_strength{ absolute_strength - relative_strength };

      return
      {
         is_active,
         is_active ? previous_absolute_strength == 0.0 : previous_absolute_strength > 0.0
      };
   }
}