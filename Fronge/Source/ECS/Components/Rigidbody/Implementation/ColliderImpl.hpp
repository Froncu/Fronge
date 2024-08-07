#if not defined COLLIDER_IMPL_HPP
#define COLLIDER_IMPL_HPP

#include "Core.hpp"
#include "ECS/Components/Rigidbody/Collider.hpp"
#include "Utility/CustomUniquePointer.hpp"

#include <box2d/box2d.h>

namespace fro
{
	class Collider::Implementation final
	{
	public:
		FRO_NODISCARD static std::unique_ptr<b2Shape> createb2Shape(
			std::variant<Rectangle<double>, Polygon<double>, Chain<double>, Circle<double>> const& shape);

		Implementation(b2Body& body, b2FixtureDef const& fixtureDefinition);

		~Implementation() = default;

		FRO_API FRO_NODISCARD b2Fixture& getb2Fixture() const;
		FRO_API FRO_NODISCARD b2FixtureDef getb2FixtureDef() const;

	private:
		Implementation(Implementation const&) = delete;
		Implementation(Implementation&&) noexcept = delete;

		Implementation& operator=(Implementation const&) = delete;
		Implementation& operator=(Implementation&&) noexcept = delete;

		CustomUniquePointer<b2Fixture> mFixture;
	};
}

#endif