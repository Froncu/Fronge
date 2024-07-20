#include "froch.hpp"

#include "GamepadImpl.hpp"
#include "SystemEventManager/SystemEventManager.hpp"

#include <SDL_gamecontroller.h>

namespace fro
{
	Gamepad::Implementation::Implementation(std::int32_t const deviceID)
		: mSDLGameController{ SDL_GameControllerOpen(deviceID), SDL_GameControllerClose }
	{
		if (mSDLGameController.get() == nullptr)
			FRO_EXCEPTION("failed to open SDL_GameControllerOpen with device ID {} ({})",
				deviceID, SDL_GetError());
	}

	SDL_GameController* Gamepad::Implementation::getSDLGameController() const
	{
		return mSDLGameController.get();
	}

	std::int32_t Gamepad::Implementation::getInstanceID() const
	{
		return SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(mSDLGameController.get()));
	}

	Gamepad::Gamepad(std::int32_t const deviceID)
		: mOnInputEvent
		{
			VariantVisitor
			{
				[this](GamepadDisconnectedEvent const& event)
				{
					if (event.instanceID not_eq mID)
						return false;

					mDeviceID = -1;
					mImplementation.reset();
					return true;
				},

				[](auto&&)
				{
					return false;
				}
			}
		}
		, mDeviceID{ deviceID }
		, mImplementation{ std::make_unique<Implementation>(mDeviceID) }
		, mID{ mImplementation->getInstanceID() }
	{
		SystemEventManager::mInputEvent.addListener(mOnInputEvent);

		Logger::info("opened Gamepad with ID {} from device with ID {}!",
			mID, mDeviceID);
	}

	Gamepad::Gamepad(Gamepad const& other)
		: mOnInputEvent{ other.mOnInputEvent }
		, mDeviceID{ other.mDeviceID }
		, mImplementation{ std::make_unique<Implementation>(mDeviceID) }
		, mID{ mImplementation->getInstanceID() }
	{
		Logger::info("opened Gamepad with ID {} from device with ID {}!",
			mID, mDeviceID);
	}

	Gamepad::Gamepad(Gamepad&& other) noexcept
		: mOnInputEvent{ std::move(other.mOnInputEvent) }
		, mDeviceID{ other.mDeviceID }
		, mImplementation{ std::move(other.mImplementation) }
		, mID{ mImplementation->getInstanceID() }
	{
		other.mDeviceID = -1;
	}

	Gamepad::~Gamepad()
	{
		Logger::info("closed Gamepad with ID {}!",
			mID);
	}

	Gamepad& Gamepad::operator=(Gamepad const& other)
	{
		if (this == &other)
			return *this;

		mOnInputEvent = other.mOnInputEvent;
		mDeviceID = other.mDeviceID;
		mImplementation = std::make_unique<Implementation>(mDeviceID);
		mID = mImplementation->getInstanceID();

		return *this;
	}

	Gamepad& Gamepad::operator=(Gamepad&& other) noexcept
	{
		if (this == &other)
			return *this;

		mOnInputEvent = std::move(other.mOnInputEvent);
		mDeviceID = other.mDeviceID;
		mImplementation = std::move(other.mImplementation);
		mID = mImplementation->getInstanceID();

		other.mDeviceID = -1;

		return *this;
	}

	std::int32_t Gamepad::getDeviceID() const
	{
		return mDeviceID;
	}

	std::int32_t Gamepad::getID() const
	{
		return mID;
	}
}