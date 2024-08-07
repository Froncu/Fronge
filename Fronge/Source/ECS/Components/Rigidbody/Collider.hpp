#if not defined COLLIDER_HPP
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

	// intentionally left out FRO_API from the constructors, operators and destructors;
	// the user should not create this outside Rigidbody
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

	private:
		std::unique_ptr<Implementation> mImplementation;
	};
}

#endif