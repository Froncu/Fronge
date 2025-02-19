#include "froch.hpp"

#include "Gamepad.hpp"
#include "Implementation/GamepadImpl.hpp"
#include "SystemEventManager/SystemEventManager.hpp"

namespace fro
{
   Gamepad::Gamepad(std::int32_t const deviceID)
      : mOnInputEvent
      {
         VariantVisitor
         {
            [smartThis = Reference{ this }](GamepadDisconnectedEvent const& event)
            {
               if (event.ID not_eq smartThis->mID)
                  return false;

               smartThis->mDeviceID = -1;
               smartThis->mImplementation.reset();
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
      , mID{ mImplementation->getID() }
   {
      Logger::info("opened Gamepad with ID {} from device with ID {}!",
         mID, mDeviceID);
   }

   Gamepad::Gamepad(Gamepad const& other)
      : Referencable(other)

      , mOnInputEvent{ other.mOnInputEvent }
      , mDeviceID{ other.mDeviceID }
      , mImplementation{ std::make_unique<Implementation>(mDeviceID) }
      , mID{ mImplementation->getID() }
   {
      Logger::info("opened Gamepad with ID {} from device with ID {}!",
         mID, mDeviceID);
   }

   Gamepad::Gamepad(Gamepad&& other) noexcept
      : Referencable(std::move(other))

      , mOnInputEvent{ std::move(other.mOnInputEvent) }
      , mDeviceID{ other.mDeviceID }
      , mImplementation{ std::move(other.mImplementation) }
      , mID{ mImplementation->getID() }
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

      Referencable::operator=(other);

      mOnInputEvent = other.mOnInputEvent;
      mDeviceID = other.mDeviceID;
      mImplementation = std::make_unique<Implementation>(mDeviceID);
      mID = mImplementation->getID();

      return *this;
   }

   Gamepad& Gamepad::operator=(Gamepad&& other) noexcept
   {
      if (this == &other)
         return *this;

      Referencable::operator=(std::move(other));

      mOnInputEvent = std::move(other.mOnInputEvent);
      mDeviceID = other.mDeviceID;
      mImplementation = std::move(other.mImplementation);
      mID = mImplementation->getID();

      other.mDeviceID = -1;

      return *this;
   }

   Gamepad::Implementation& Gamepad::getImplementation() const
   {
      return *mImplementation;
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