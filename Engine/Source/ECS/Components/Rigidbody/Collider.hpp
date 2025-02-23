#ifndef COLLIDER_HPP
#define COLLIDER_HPP

#include "froch.hpp"

#include "Reference/Referencable.hpp"
#include "Maths/Structs/Chain.hpp"
#include "Maths/Structs/Circle.hpp"
#include "Maths/Structs/Polygon.hpp"
#include "Maths/Structs/Rectangle.hpp"

namespace fro
{
	class Rigidbody;

	// NOTE: intentionally left out FRO_API from the constructors, operators and destructors;
	// the user should not create this outside fro::Rigidbody
	class Collider final : public Referencable
	{
	public:
		class Implementation;

		Collider(std::unique_ptr<Implementation>&& implementation);

		Collider(Collider const& other);
		Collider(Collider&& other) noexcept;

		~Collider();

		Collider& operator=(Collider const& other);
		Collider& operator=(Collider&& other) noexcept;

		FRO_API FRO_NODISCARD Implementation& getImplementation() const;

		FRO_API void setShape(std::variant<Rectangle<double>, Polygon<double>, Chain<double>, Circle<double>> const& shape);
		FRO_API void setDensity(double const density);
		FRO_API void setRestitution(double const restitution);
		FRO_API void setFriction(double const friction);
		FRO_API void setSensor(bool const isSensor);

		FRO_API void setCategoryBits(std::uint16_t const categoryBits);
		FRO_API void setMaskBits(std::uint16_t const maskBits);
		FRO_API void setGroupIndex(std::int16_t const groupIndex);

		FRO_API FRO_NODISCARD std::uint16_t getCategoryBits() const;
		FRO_API FRO_NODISCARD std::uint16_t getMaskBits() const;
		FRO_API FRO_NODISCARD std::int16_t getGroupIndex() const;

	private:
		std::unique_ptr<Implementation> mImplementation;
	};
}

#endif