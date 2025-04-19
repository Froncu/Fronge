#include "froch.hpp"
#include "user_input.hpp"

namespace fro
{
   UserInput::UserInput(int const id)
      : id_{ id }
   {
   }

   bool UserInput::operator==(int const user_index) const
   {
      return id() == user_index;
   }

   bool UserInput::operator==(UserInput const& other) const
   {
      return *this == other.id();
   }

   // void UserInput::assign_gamepad(UserInput const& source_user_input)
   // {
   //    if (id() == GLOBAL_USER_INDEX)
   //       exception("attempted to assign the gamepad from UserInput{} to the global user"
   //          "(the global user listens to all the input devices; it doesn't have any input devices assigned)",
   //          source_user_input.id());
   //
   //    if (source_user_input == GLOBAL_USER_INDEX)
   //       exception("attempted to assign the gamepad from the global user to UserInput{}"
   //          "(the global user listens to all the input devices; it doesn't have any input devices assigned)",
   //          id());
   //
   //    if (source_user_input == id())
   //       return;
   //
   //    std::optional<Gamepad>& source_gamepad{ source_user_input.gamepad_ };
   //    if (not source_gamepad.has_value())
   //       exception("attempted to assign the gamepad from UserInput{} to UserInput{}"
   //          "(UserInput{} does not have a gamepad assigned)",
   //          source_user_input.id(), id(), source_user_input.id());
   //
   //    source_gamepad->assign_user_input(id());
   //    if (gamepad_.has_value())
   //       gamepad_->assign_user_input(source_user_input.id());
   //
   //    std::swap(source_gamepad, gamepad_);
   //
   //    std::unordered_set<Input> swapped_inputs{};
   //    auto const predicate{
   //       [](Input const input)
   //       {
   //          return std::holds_alternative<GamepadButton>(input) or std::holds_alternative<GamepadAxis>(input);
   //       }
   //    };
   //
   //    for (auto& [input, strength_a] : source_user_input.input_strengths_)
   //       if (predicate(input))
   //       {
   //          double& strength_b{ source.input_strengths_[input] };
   //          if (strength_a == strength_b)
   //             continue;
   //
   //          std::swap(strength_a, strength_b);
   //          swapped_inputs.insert(input);
   //       }
   //
   //    for (auto& [input, strength_b] : source_user_input.input_strengths_)
   //       if (not swapped_inputs.contains(input) and predicate(input))
   //       {
   //          double& strength_a{ source_user_input.input_strengths_[input] };
   //          if (strength_b == strength_a)
   //             continue;
   //
   //          std::swap(strength_b, strength_a);
   //          swapped_inputs.insert(input);
   //       }
   // }

   EventDispatcher<double const>& UserInput::bind_action(std::string const& action_name, ValueAction action) const
   {
      InternalValueAction& internal_action{ value_actions_[action_name] };
      internal_action.action = std::move(action);
      return internal_action.value_changed_event;
   }

   EventDispatcher<double const>& UserInput::bind_action(std::string const& action_name, AxisAction action) const
   {
      InternalAxisAction& internal_action{ axis_actions_[action_name] };
      internal_action.action = std::move(action);
      return internal_action.value_changed_event;
   }

   EventDispatcher<Vector2<double> const>& UserInput::bind_action(std::string const& action_name, VectorAction action) const
   {
      InternalVectorAction& internal_action{ vector_actions_[action_name] };
      internal_action.action = std::move(action);
      return internal_action.value_changed_event;
   }

   EventDispatcher<double const>& UserInput::value_action(std::string const& action_name) const
   {
      return value_actions_[action_name].value_changed_event;
   }

   EventDispatcher<double const>& UserInput::axis_action(std::string const& action_name) const
   {
      return axis_actions_[action_name].value_changed_event;
   }

   EventDispatcher<Vector2<double> const>& UserInput::vector_action(std::string const& action_name) const
   {
      return vector_actions_[action_name].value_changed_event;
   }

   double UserInput::value_action_strength(std::string const& action_name) const
   {
      return value_actions_[action_name].value;
   }

   double UserInput::axis_action_strength(std::string const& action_name) const
   {
      return axis_actions_[action_name].value;
   }

   Vector2<double> UserInput::vector_action_strength(std::string const& action_name) const
   {
      return vector_actions_[action_name].value;
   }

   int UserInput::id() const
   {
      return id_;
   }
}