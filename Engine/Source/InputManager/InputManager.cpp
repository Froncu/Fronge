#include <SDL.h>

#include "froch.hpp"
#include "InputManager.hpp"
#include "SystemEventManager/SystemEventManager.hpp"

namespace fro
{
   InputManager::InputManager()
   {
      Logger::info("initialized InputManager!");
   }

   InputManager::~InputManager()
   {
      Logger::info("shut down InputManager!");
   }

   void InputManager::processInputContinous()
   {
      for (auto& [input, info] : mInputs)
         info.relativeStrength = 0.0;

      for (auto& [input, info] : mActions)
         info.relativeStrength = 0.0;
   }

   void InputManager::bindActionToInput(std::string const& actionName, Input const input)
   {
      mActions[actionName].boundInputs.insert(input);
   }

   void InputManager::setActionDeadzone(std::string const& actionName, double const deadzone)
   {
      mActions[actionName].deadzone = deadzone;
   }

   double InputManager::getInputStrength(Input const input)
   {
      return mInputs[input].absoluteStrength;
   }

   double InputManager::getActionStrength(std::string const& actionName)
   {
      return mActions[actionName].absoluteStrength;
   }

   double InputManager::getActionStrengthAxis1D(std::string const& positiveActionName, std::string const& negativeActionName)
   {
      return getActionStrength(positiveActionName) - getActionStrength(negativeActionName);
   }

   Vector2<double> InputManager::getActionStrengthAxis2D(
      std::string const& positiveActionNameX,
      std::string const& negativeActionNameX,
      std::string const& positiveActionNameY,
      std::string const& negativeActionNameY)
   {
      Vector2<double> const actionAxis2D
      {
         mActions[positiveActionNameX].rawStrength - mActions[negativeActionNameX].rawStrength,
         mActions[positiveActionNameY].rawStrength - mActions[negativeActionNameY].rawStrength
      };

      std::array<double, 4> const actionDeadzones
      {
         mActions[positiveActionNameX].deadzone,
         mActions[negativeActionNameX].deadzone,
         mActions[positiveActionNameY].deadzone,
         mActions[negativeActionNameY].deadzone
      };

      auto const avarageDeadzoneMagnitude
      {
         std::reduce(actionDeadzones.begin(), actionDeadzones.end(), 0.0) / actionDeadzones.size()
      };

      double const actionAxis2DMagnitude{ actionAxis2D.getMagnitude() };

      if (actionAxis2DMagnitude <= avarageDeadzoneMagnitude)
         return {};

      if (actionAxis2DMagnitude > 1.0)
         return actionAxis2D / actionAxis2DMagnitude;

      double const inverseLerpedLength{ (actionAxis2DMagnitude - avarageDeadzoneMagnitude) / (1.0 - avarageDeadzoneMagnitude) };
      return actionAxis2D * (inverseLerpedLength / actionAxis2DMagnitude);
   }

   bool InputManager::isInputJustPressed(Input const input)
   {
      auto const& [absoluteStrength, relativeStrength]{ mInputs[input] };

      return isJustPressed(absoluteStrength, relativeStrength);
   }

   bool InputManager::isActionJustPressed(std::string const& actionName)
   {
      auto const& [boundInputs, rawStrength, absoluteStrength, relativeStrength, deadzone]{ mActions[actionName] };

      return isJustPressed(absoluteStrength, relativeStrength);
   }

   bool InputManager::isInputJustReleased(Input const input)
   {
      auto const& [absoluteStrength, relativeStrength]{ mInputs[input] };

      return isJustReleased(absoluteStrength, relativeStrength);
   }

   bool InputManager::isActionJustReleased(std::string const& actionName)
   {
      auto const& [boundInputs, rawStrength, absoluteStrength, relativeStrength, deadzone]{ mActions[actionName] };

      return isJustReleased(absoluteStrength, relativeStrength);
   }

   void InputManager::setInputStrength(Input const input, double const newStrength)
   {
      auto& [absoluteStrength, relativeStrength]{ mInputs[input] };

      relativeStrength = newStrength - absoluteStrength;
      absoluteStrength = newStrength;

      if (auto const [isActive, didJustChange]{ getStrengthInfo(absoluteStrength, relativeStrength) }; didJustChange)
      {
         if (isActive)
            mInputPressedEvent.notify(input, absoluteStrength);
         else
            mInputReleasedEvent.notify(input, absoluteStrength);
      }

      for (auto&& [actionName, actionInfo] : mActions)
         if (actionInfo.boundInputs.contains(input))
         {
            double largestBoundStrength{};

            if (absoluteStrength > actionInfo.rawStrength)
               largestBoundStrength = absoluteStrength;
            else
               for (Input const boundInput : actionInfo.boundInputs)
               {
                  largestBoundStrength = std::max(getInputStrength(boundInput), largestBoundStrength);
                  if (largestBoundStrength == 1.0)
                     break;
               };

            actionInfo.rawStrength = largestBoundStrength;

            double const strength{ deadzonedStrength(actionInfo.rawStrength, actionInfo.deadzone) };
            actionInfo.relativeStrength = strength - actionInfo.absoluteStrength;
            actionInfo.absoluteStrength = strength;

            if (auto const [isActive, didJustChange]{
               getStrengthInfo(actionInfo.absoluteStrength, actionInfo.relativeStrength)
            }; didJustChange)
            {
               if (isActive)
                  mActionPressedEvent.notify(actionName, absoluteStrength);
               else
                  mActionReleasedEvent.notify(actionName, absoluteStrength);
            }
         }
   }

   double InputManager::deadzonedStrength(double const strength, double const deadzone)
   {
      return
      {
         strength <= deadzone ? 0.0 : (strength - deadzone) / (1.0 - deadzone)
      };
   }

   bool InputManager::isJustPressed(double const absoluteStrength, double const relativeStrength)
   {
      auto const [isActive, didJustChange]{ getStrengthInfo(absoluteStrength, relativeStrength) };
      return isActive and didJustChange;
   }

   bool InputManager::isJustReleased(double const absoluteStrength, double const relativeStrength)
   {
      auto const [isActive, didJustChange]{ getStrengthInfo(absoluteStrength, relativeStrength) };
      return not isActive and didJustChange;
   }

   InputManager::StrengthInfo InputManager::getStrengthInfo(double const absoluteStrength, double const relativeStrength)
   {
      bool const isActive{ absoluteStrength > 0.0 };
      double const previousAbsoluteStrength{ absoluteStrength - relativeStrength };

      return
      {
         isActive,
         isActive ? previousAbsoluteStrength == 0.0 : previousAbsoluteStrength > 0.0
      };
   }
}