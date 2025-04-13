#ifndef INPUT_MANAGER_HPP
#define INPUT_MANAGER_HPP

#include "core.hpp"
#include "events/input_event.hpp"
#include "events/observer/event_listener.hpp"
#include "froch.hpp"
#include "input/input.hpp"
#include "maths/vector2.hpp"
#include "services/locator.hpp"
#include "services/system_event_dispatcher/system_event_dispatcher.hpp"
#include "utility/variant_visitor.hpp"

namespace fro
{
   class InputManager final
   {
      struct InputInfo final
      {
         double absolute_strength{};
         double relative_strength{};
      };

      struct ActionInfo final
      {
         std::unordered_set<Input> bound_inputs{};

         double raw_strength{};
         double absolute_strength{};
         double relative_strength{};
         double deadzone{ 0.25 };
      };

      struct StrengthInfo final
      {
         bool is_active;
         bool did_just_change;
      };

      public:
         InputManager() = default;
         InputManager(InputManager const&) = default;
         InputManager(InputManager&&) = default;

         ~InputManager() = default;

         InputManager& operator=(InputManager const&) = default;
         InputManager& operator=(InputManager&&) = default;

         FRO_API void process_input_continous();

         FRO_API void bind_action_to_input(std::string const& action_name, Input const& input);
         FRO_API void change_action_deadzone(std::string const& action_name, double deadzone);

         FRO_API [[nodiscard]] double input_strength(Input const& input);
         FRO_API [[nodiscard]] double action_strength(std::string const& action_name);
         FRO_API [[nodiscard]] double action_strength_axis_1d(
            std::string const& positive_action_name,
            std::string const& negative_action_name);
         FRO_API [[nodiscard]] Vector2<double> action_strength_axis_2d(
            std::string const& positive_action_name_x,
            std::string const& negative_action_name_x,
            std::string const& positive_action_name_y,
            std::string const& negative_action_name_y);

         FRO_API [[nodiscard]] bool input_just_activated(Input const& input);
         FRO_API [[nodiscard]] bool action_just_activated(std::string const& action_name);
         FRO_API [[nodiscard]] bool input_just_deactivated(Input const& input);
         FRO_API [[nodiscard]] bool action_just_deactivated(std::string const& action_name);

         EventDispatcher<Input const, double const> input_activated_event{};
         EventDispatcher<Input const, double const> input_deactivated_event{};
         EventDispatcher<std::string const, double const> action_activated_event{};
         EventDispatcher<std::string const, double const> action_deactivated_event{};

         EventListener<InputEvent const> on_input_event
         {
            VariantVisitor
            {
               [this](MouseButtonDownEvent const& event)
               {
                  set_input_strength(event.button, 1.0);
                  return true;
               },

               [this](MouseButtonUpEvent const& event)
               {
                  set_input_strength(event.button, 0.0);
                  return true;
               },

               [this](KeyDownEvent const& event)
               {
                  set_input_strength(event.key, 1.0);
                  return true;
               },

               [this](KeyUpEvent const& event)
               {
                  set_input_strength(event.key, 0.0);
                  return true;
               },

               [this](GamepadButtonDownEvent const& event)
               {
                  set_input_strength(event.button, 1.0);
                  return true;
               },

               [this](GamepadButtonUpEvent const& event)
               {
                  set_input_strength(event.button, 0.0);
                  return true;
               },

               [this](GamepadAxisEvent const& event)
               {
                  set_input_strength(event.axis, event.value);
                  return true;
               },

               [](auto&&)
               {
                  return false;
               }
            }, Locator::get<SystemEventDispatcher>().input_event
         };

      private:
         FRO_API void set_input_strength(Input const& input, double new_strength);

         [[nodiscard]] static double deadzoned_strength(double strength, double deadzone);
         [[nodiscard]] static bool is_just_activated(double absolute_strength, double relative_strength);
         [[nodiscard]] static bool is_just_deactivated(double absolute_strength, double relative_strength);
         [[nodiscard]] static StrengthInfo strength_info(double absolute_strength, double relative_strength);

         std::unordered_map<Input, InputInfo> inputs_{};
         std::unordered_map<std::string, ActionInfo> actions_{};
   };
}

#endif