#ifndef INPUT_MANAGER_HPP
#define INPUT_MANAGER_HPP

#include "Core.hpp"
#include "Events/InputEvent.hpp"
#include "Events/Systems/EventListener.hpp"
#include "Events/Systems/UniqueEventQueue.hpp"
#include "froch.hpp"
#include "Input/Input.hpp"
#include "Maths/Structs/Vector2.hpp"
#include "Utility/CustomUniquePointer.hpp"
#include "Utility/VariantVisitor.hpp"

namespace fro
{
   class InputManager final
   {
      struct InputInfo final
      {
         public:
            double absoluteStrength{};
            double relativeStrength{};
      };

      struct ActionInfo final
      {
         public:
            std::unordered_set<Input> boundInputs{};

            double rawStrength{};
            double absoluteStrength{};
            double relativeStrength{};
            double deadzone{ 0.25 };
      };

      struct StrengthInfo final
      {
         public:
            bool isActive;
            bool didJustChange;
      };

      public:
         FRO_API InputManager();
         InputManager(InputManager const&) = delete;
         InputManager(InputManager&&) noexcept = delete;

         FRO_API ~InputManager();

         InputManager& operator=(InputManager const&) = delete;
         InputManager& operator=(InputManager&&) noexcept = delete;

         FRO_API void processInputContinous();

         FRO_API void bindActionToInput(std::string const& actionName, Input const input);
         FRO_API void setActionDeadzone(std::string const& actionName, double const deadzone);

         FRO_API FRO_NODISCARD double getInputStrength(Input const input);
         FRO_API FRO_NODISCARD double getActionStrength(std::string const& actionName);
         FRO_API FRO_NODISCARD double getActionStrengthAxis1D(
            std::string const& positiveActionName,
            std::string const& negativeActionName);
         FRO_API FRO_NODISCARD Vector2<double> getActionStrengthAxis2D(
            std::string const& positiveActionNameX,
            std::string const& negativeActionNameX,
            std::string const& positiveActionNameY,
            std::string const& negativeActionNameY);

         FRO_API FRO_NODISCARD bool isInputJustPressed(Input const input);
         FRO_API FRO_NODISCARD bool isActionJustPressed(std::string const& actionName);
         FRO_API FRO_NODISCARD bool isInputJustReleased(Input const input);
         FRO_API FRO_NODISCARD bool isActionJustReleased(std::string const& actionName);

         EventDispatcher<Input const, double const> mInputPressedEvent{};
         EventDispatcher<Input const, double const> mInputReleasedEvent{};
         EventDispatcher<std::string const, double const> mActionPressedEvent{};
         EventDispatcher<std::string const, double const> mActionReleasedEvent{};

         EventListener<InputEvent const> mOnInputEvent
         {
            VariantVisitor
            {
               [this](MouseButtonDownEvent const& event)
               {
                  setInputStrength(event.button, 1.0);
                  return true;
               },

               [this](MouseButtonUpEvent const& event)
               {
                  setInputStrength(event.button, 0.0);
                  return true;
               },

               [this](KeyDownEvent const& event)
               {
                  setInputStrength(event.key, 1.0);
                  return true;
               },

               [this](KeyUpEvent const& event)
               {
                  setInputStrength(event.key, 0.0);
                  return true;
               },

               [this](GamepadButtonDownEvent const& event)
               {
                  setInputStrength(event.input, 1.0);
                  return true;
               },

               [this](GamepadButtonUpEvent const& event)
               {
                  setInputStrength(event.input, 0.0);
                  return true;
               },

               [this](GamepadAxisEvent const& event)
               {
                  setInputStrength(event.input, event.value);
                  return true;
               },

               [](auto&&)
               {
                  return false;
               }
            }
         };

      private:
         void setInputStrength(Input const input, double const newStrength);

         FRO_NODISCARD double deadzonedStrength(double const strength, double const deadzone);

         FRO_NODISCARD bool isJustPressed(double const absoluteStrength, double const relativeStrength);
         FRO_NODISCARD bool isJustReleased(double const absoluteStrength, double const relativeStrength);
         FRO_NODISCARD StrengthInfo getStrengthInfo(double const absoluteStrength, double const relativeStrength);

         std::unordered_map<Input, InputInfo> mInputs{};
         std::unordered_map<std::string, ActionInfo> mActions{};
   };
}

#endif