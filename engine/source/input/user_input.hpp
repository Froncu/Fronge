#ifndef USER_INPUT_HPP
#define USER_INPUT_HPP

#include "actions.hpp"
#include "events/observer/event_dispatcher.hpp"
#include "gamepad.hpp"
#include "maths/vector2.hpp"
#include "reference/referenceable.hpp"

namespace fro
{
   class UserInput final : public Referenceable
   {
      friend class InputManager;

      struct InternalValueAction final
      {
         ValueAction action{};
         EventDispatcher<double const> value_changed_event{};
         double value{};
      };

      struct InternalAxisAction final
      {
         AxisAction action{};
         EventDispatcher<double const> value_changed_event{};
         double value{};
      };

      struct InternalVectorAction final
      {
         VectorAction action{};
         EventDispatcher<Vector2<double> const> value_changed_event{};
         Vector2<double> value{};
      };

      public:
         static int constexpr GLOBAL_USER_ID{ -1 };

         UserInput(UserInput const&) = delete;
         UserInput(UserInput&&) = default;

         virtual ~UserInput() override = default;

         UserInput& operator=(UserInput const&) = delete;
         UserInput& operator=(UserInput&&) = default;
         FRO_API [[nodiscard]] bool operator==(int id) const;
         FRO_API [[nodiscard]] bool operator==(UserInput const& other) const;

         FRO_API EventDispatcher<double const>& bind_action(std::string const& action_name, ValueAction action) const;
         FRO_API EventDispatcher<double const>& bind_action(std::string const& action_name, AxisAction action) const;
         FRO_API EventDispatcher<Vector2<double> const>& bind_action(std::string const& action_name, VectorAction action) const;

         FRO_API [[nodiscard]] EventDispatcher<double const>& value_action(std::string const& action_name) const;
         FRO_API [[nodiscard]] EventDispatcher<double const>& axis_action(std::string const& action_name) const;
         FRO_API [[nodiscard]] EventDispatcher<Vector2<double> const>& vector_action(std::string const& action_name) const;

         FRO_API [[nodiscard]] double value_action_strength(std::string const& action_name) const;
         FRO_API [[nodiscard]] double axis_action_strength(std::string const& action_name) const;
         FRO_API [[nodiscard]] Vector2<double> vector_action_strength(std::string const& action_name) const;
         FRO_API [[nodiscard]] int id() const;

      private:
         [[nodiscard]] static double deadzoned_strength(double strength, double deadzone);
         [[nodiscard]] static double highest_strength(std::unordered_set<Input> const& inputs,
            std::unordered_map<Input, double>& input_strengths);

         explicit UserInput(int id);

         void calculate_action_values_if(std::function<bool(std::unordered_set<Input> const&)> const& predicate) const;
         void reset_input_strength_if(std::function<bool(Input)> const& predicate) const;
         void copy_input_strength_if(UserInput const& user_input, std::function<bool(Input)> const& predicate) const;
         void move_input_strength_if(UserInput const& user_input, std::function<bool(Input)> const& predicate) const;
         void swap_input_strengths_if(UserInput const& user_input, std::function<bool(Input)> const& predicate) const;
         void change_input_strength(Input input, double strength) const;

         mutable std::unordered_map<Input, double> input_strengths_{};
         mutable std::unordered_map<std::string, InternalValueAction> value_actions_{};
         mutable std::unordered_map<std::string, InternalAxisAction> axis_actions_{};
         mutable std::unordered_map<std::string, InternalVectorAction> vector_actions_{};

         int id_;
   };
}

template <>
struct std::hash<fro::UserInput>
{
   using is_transparent = void;

   [[nodiscard]] std::size_t operator()(fro::UserInput const& user_input) const noexcept
   {
      return user_input.id();
   }

   [[nodiscard]] std::size_t operator()(int const id) const noexcept
   {
      return id;
   }
};

template <>
struct std::equal_to<fro::UserInput>
{
   using is_transparent = void;

   [[nodiscard]] bool operator()(fro::UserInput const& user_input_a, fro::UserInput const& user_input_b) const noexcept
   {
      return user_input_a == user_input_b;
   }

   [[nodiscard]] bool operator()(int const id, fro::UserInput const& user_input) const noexcept
   {
      return user_input == id;
   }
};

#endif