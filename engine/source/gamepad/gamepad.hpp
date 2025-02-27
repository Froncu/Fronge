#ifndef GAMEPAD_HPP
#define GAMEPAD_HPP

#include "froch.hpp"

#include "Core.hpp"
#include "Events/Systems/EventListener.hpp"
#include "Events/InputEvent.hpp"
#include "Reference/Reference.hpp"
#include "Utility/VariantVisitor.hpp"

namespace fro
{
   class Gamepad final : public Referencable
   {
      public:
         class Implementation;

         FRO_API Gamepad(std::int32_t const deviceID);
         FRO_API Gamepad(Gamepad const& other);
         FRO_API Gamepad(Gamepad&& other) noexcept;

         FRO_API ~Gamepad();

         FRO_API Gamepad& operator=(Gamepad const& other);
         FRO_API Gamepad& operator=(Gamepad&& other) noexcept;

         FRO_API FRO_NODISCARD Implementation& getImplementation() const;

         FRO_API FRO_NODISCARD std::int32_t getDeviceID() const;
         FRO_API FRO_NODISCARD std::int32_t getID() const;

         EventListener<InputEvent const> mOnInputEvent;

      private:
         std::int32_t mDeviceID;
         std::unique_ptr<Implementation> mImplementation;
         std::int32_t mID;
   };
}

#endif