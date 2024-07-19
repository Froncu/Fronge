#if not defined INPUT_MANAGER_HPP
#define INPUT_MANAGER_HPP

#include "froch.hpp"

#include "Core.hpp"
#include "Events/InputEvent.hpp"
#include "Events/Systems/UniqueEventQueue.hpp"
#include "Events/Systems/EventListener.hpp"
#include "Input/Input.hpp"
#include "Maths/MathStructs.hpp"
#include "Utility/CustomUniquePointer.hpp"

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
			std::set<InputInfo const*> boundInputInfos{};

			double simulatedAbsoluteStrength{};
			double simulatedRelativeStrength{};

			double deadzone{ 0.25 };
		};

		struct SimulateActionStrengthEvent final
		{
		public:
			FRO_NODISCARD bool operator==(SimulateActionStrengthEvent const& other) const;

			ActionInfo* actionInfo{};
			double simulatedStrength{};
		};

	public:
		FRO_API static void initialize();

		FRO_API static void processInputContinous();

		FRO_API static void bindActionToInput(std::string const& actionName, Input const input);
		FRO_API void static setActionDeadzone(std::string const& actionName, double const deadzone);
		FRO_API void static simulateActionStrength(std::string const& actionName, double const strength);

		FRO_API FRO_NODISCARD static double getInputStrength(Input const input);
		FRO_API FRO_NODISCARD static double getActionStrength(std::string const& actionName);
		FRO_API FRO_NODISCARD static double getActionStrengthAxis1D(
			std::string const& positiveActionName,
			std::string const& negativeActionName);

		FRO_API FRO_NODISCARD static Vector2<double> getActionStrengthAxis2D(
			std::string const& positiveActionNameX,
			std::string const& negativeActionNameX,
			std::string const& positiveActionNameY,
			std::string const& negativeActionNameY);

		FRO_API FRO_NODISCARD static bool isInputJustPressed(Input const input);
		FRO_API FRO_NODISCARD static bool isActionJustPressed(std::string const& actionName);
		FRO_API FRO_NODISCARD static bool isInputJustReleased(Input const input);
		FRO_API FRO_NODISCARD static bool isActionJustReleased(std::string const& actionName);

	private:
		static void setInputStrength(Input const input, double const newStrength);

		FRO_NODISCARD static std::pair<bool, bool> getStrengthInfo(
			double const absoluteStrength,
			double const relativeStrength,
			double const deadzone);

		FRO_NODISCARD static double getLargestStrength(
			double const strength,
			double const largestStrength,
			double const deadzone);

		FRO_NODISCARD static double getActionStrength(std::string const& actionName, double const deadzone);

		static EventListener<InputEvent const> sOnInputEvent;
		static UniqueEventQueue<SimulateActionStrengthEvent> sSimulateActionStrengthEvents;
		static std::map<Input, InputInfo> sInputs;
		static std::map<std::string, ActionInfo> sActions;

		InputManager() = delete;
		InputManager(InputManager const&) = delete;
		InputManager(InputManager&&) noexcept = delete;

		~InputManager() = delete;

		InputManager& operator=(InputManager const&) = delete;
		InputManager& operator=(InputManager&&) noexcept = delete;
	};
}

#endif