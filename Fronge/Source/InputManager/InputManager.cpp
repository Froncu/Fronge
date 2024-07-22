#include "froch.hpp"

#include "InputManager.hpp"
#include "SystemEventManager/SystemEventManager.hpp"
#include "Utility/VariantVisitor.hpp"
#include "Maths/MathFunctions.hpp"

#include <SDL.h>

namespace fro
{
	void InputManager::initialize()
	{
		SystemEventManager::mInputEvent.addListener(sOnInputEvent);

		Logger::info("initialized InputManager!");
	}

	void InputManager::shutDown()
	{
		SystemEventManager::mInputEvent.removeListener(sOnInputEvent);

		Logger::info("shut down InputManager!");
	}

	void InputManager::processInputContinous()
	{
		for (auto& [input, info] : sInputs)
			info.relativeStrength = 0.0;

		for (auto& [input, info] : sActions)
			info.relativeStrength = 0.0;
	}

	void InputManager::bindActionToInput(std::string const& actionName, Input const input)
	{
		sActions[actionName].boundInputs.insert(input);
	}

	void InputManager::setActionDeadzone(std::string const& actionName, double const deadzone)
	{
		sActions[actionName].deadzone = deadzone;
	}

	double InputManager::getInputStrength(Input const input)
	{
		return sInputs[input].absoluteStrength;
	}

	double InputManager::getActionStrength(std::string const& actionName)
	{
		return sActions[actionName].absoluteStrength;
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
			sActions[positiveActionNameX].rawStrength - sActions[negativeActionNameX].rawStrength,
			sActions[positiveActionNameY].rawStrength - sActions[negativeActionNameY].rawStrength
		};

		std::array<double, 4> const actionDeadzones
		{
			sActions[positiveActionNameX].deadzone,
			sActions[negativeActionNameX].deadzone,
			sActions[positiveActionNameY].deadzone,
			sActions[negativeActionNameY].deadzone
		};

		double const avarageDeadzoneMagnitude
		{
			std::reduce(actionDeadzones.begin(), actionDeadzones.end(), 0.0) / actionDeadzones.size()
		};

		double const actionAxis2DMagnitude{ math::getMagnitude(actionAxis2D) };

		if (actionAxis2DMagnitude <= avarageDeadzoneMagnitude)
			return {};

		if (actionAxis2DMagnitude > 1.0)
			return actionAxis2D / actionAxis2DMagnitude;

		double const inverseLerpedLength{ (actionAxis2DMagnitude - avarageDeadzoneMagnitude) / (1.0 - avarageDeadzoneMagnitude) };
		return actionAxis2D * (inverseLerpedLength / actionAxis2DMagnitude);
	}

	bool InputManager::isInputJustPressed(Input const input)
	{
		auto const& [absoluteStrength, relativeStrength] { sInputs[input] };

		return isJustPressed(absoluteStrength, relativeStrength);
	}

	bool InputManager::isActionJustPressed(std::string const& actionName)
	{
		auto const& [boundInputs, rawStrength, absoluteStrength, relativeStrength, deadzone] { sActions[actionName] };

		return isJustPressed(absoluteStrength, relativeStrength);
	}

	bool InputManager::isInputJustReleased(Input const input)
	{
		auto const& [absoluteStrength, relativeStrength] { sInputs[input] };

		return isJustReleased(absoluteStrength, relativeStrength);
	}

	bool InputManager::isActionJustReleased(std::string const& actionName)
	{
		auto const& [boundInputs, rawStrength, absoluteStrength, relativeStrength, deadzone] { sActions[actionName] };

		return isJustReleased(absoluteStrength, relativeStrength);
	}

	void InputManager::setInputStrength(Input const input, double const newStrength)
	{
		auto& [absoluteStrength, relativeStrength] { sInputs[input] };

		relativeStrength = newStrength - absoluteStrength;
		absoluteStrength = newStrength;

		if (auto const [isActive, didJustChange] { getStrengthInfo(absoluteStrength, relativeStrength) };
			didJustChange)
		{
			if (isActive)
				mInputPressedEvent.notify(input, absoluteStrength);
			else
				mInputReleasedEvent.notify(input, absoluteStrength);
		}

		for (auto&& [actionName, actionInfo] : sActions)
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

				double const deadzonedStrength{ deadzoneStrength(actionInfo.rawStrength, actionInfo.deadzone) };
				actionInfo.relativeStrength = deadzonedStrength - actionInfo.absoluteStrength;
				actionInfo.absoluteStrength = deadzonedStrength;

				if (auto const [isActive, didJustChange] { getStrengthInfo(actionInfo.absoluteStrength, actionInfo.relativeStrength) };
					didJustChange)
				{
					if (isActive)
						mActionPressedEvent.notify(actionName, absoluteStrength);
					else
						mActionReleasedEvent.notify(actionName, absoluteStrength);
				}
			}
	}

	double InputManager::deadzoneStrength(double const strength, double const deadzone)
	{
		return
		{
			strength <= deadzone ?
			0.0 :
			(strength - deadzone) / (1.0 - deadzone)
		};
	}

	bool InputManager::isJustPressed(double const absoluteStrength, double const relativeStrength)
	{
		auto const [isActive, didJustChange] { getStrengthInfo(absoluteStrength, relativeStrength) };
		return isActive and didJustChange;
	}

	bool InputManager::isJustReleased(double const absoluteStrength, double const relativeStrength)
	{
		auto const [isActive, didJustChange] { getStrengthInfo(absoluteStrength, relativeStrength) };
		return not isActive and didJustChange;
	}

	InputManager::StrengthInfo InputManager::getStrengthInfo(double const absoluteStrength, double const relativeStrength)
	{
		bool const isActive{ absoluteStrength > 0.0 };
		double const previousAbsoluteStrength{ absoluteStrength - relativeStrength };

		return
		{
			isActive,
			isActive ?
			previousAbsoluteStrength == 0.0 :
			previousAbsoluteStrength > 0.0
		};
	}

	EventDispatcher<Input const, double const> InputManager::mInputPressedEvent{};
	EventDispatcher<Input const, double const> InputManager::mInputReleasedEvent{};
	EventDispatcher<std::string const, double const> InputManager::mActionPressedEvent{};
	EventDispatcher<std::string const, double const> InputManager::mActionReleasedEvent{};

	EventListener<InputEvent const> InputManager::sOnInputEvent
	{
		VariantVisitor
		{
			[](MouseButtonDownEvent const& event)
			{
				setInputStrength(event.button, 1.0);
				return true;
			},

			[](MouseButtonUpEvent const& event)
			{
				setInputStrength(event.button, 0.0);
				return true;
			},

			[](KeyDownEvent const& event)
			{
				setInputStrength(event.key, 1.0);
				return true;
			},

			[](KeyUpEvent const& event)
			{
				setInputStrength(event.key, 0.0);
				return true;
			},

			[](GamepadButtonDownEvent const& event)
			{
				setInputStrength(event.input, 1.0);
				return true;
			},

			[](GamepadButtonUpEvent const& event)
			{
				setInputStrength(event.input, 0.0);
				return true;
			},

			[](GamepadAxisEvent const& event)
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

	std::map<Input, InputManager::InputInfo> InputManager::sInputs{};
	std::map<std::string, InputManager::ActionInfo> InputManager::sActions{};
}