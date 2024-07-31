#if not defined GAMEPAD_HPP
#define GAMEPAD_HPP

#include "froch.hpp"

#include "Core.hpp"
#include "Events/Systems/EventListener.hpp"
#include "Events/InputEvent.hpp"
#include "Utility/VariantVisitor.hpp"

namespace fro
{
	class Gamepad final
	{
		class Implementation;

	public:
		FRO_API Gamepad(std::int32_t const deviceID);
		FRO_API Gamepad(Gamepad const& other);
		FRO_API Gamepad(Gamepad&& other) noexcept;

		FRO_API ~Gamepad();

		FRO_API Gamepad& operator=(Gamepad const& other);
		FRO_API Gamepad& operator=(Gamepad&& other) noexcept;

		FRO_API FRO_NODISCARD Implementation& getImplementation() const;

		FRO_API FRO_NODISCARD std::int32_t getDeviceID() const;
		FRO_API FRO_NODISCARD std::int32_t getID() const;

	private:
		EventListener<InputEvent const> mOnInputEvent;
		std::int32_t mDeviceID;
		std::unique_ptr<Implementation> mImplementation;
		std::int32_t mID;
	};
}

#endif