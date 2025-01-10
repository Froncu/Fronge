#ifndef RIGIDBODY_IMPL_HPP
#define RIGIDBODY_IMPL_HPP

#include "Core.hpp"
#include "ECS/Components/Rigidbody/Rigidbody.hpp"
#include "Utility/CustomUniquePointer.hpp"

#include <box2d/b2_body.h>

namespace fro
{
	class Rigidbody::Implementation final
	{
	public:
		FRO_API Implementation(b2BodyDef const& bodyDefinition = {});

		~Implementation() = default;

		FRO_API FRO_NODISCARD b2Body& getb2Body() const;
		FRO_API FRO_NODISCARD b2BodyDef getb2BodyDef() const;

	private:
		Implementation(Implementation const&) = delete;
		Implementation(Implementation&&) noexcept = delete;

		Implementation& operator=(Implementation const&) = delete;
		Implementation& operator=(Implementation&&) noexcept = delete;

		CustomUniquePointer<b2Body> mBody;
	};
}

#endif