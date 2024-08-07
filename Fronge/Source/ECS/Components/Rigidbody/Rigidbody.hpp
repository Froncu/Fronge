#if not defined RIGIDBODY_HPP
#define RIGIDBODY_HPP

#include "froch.hpp"

#include "ECS/Components/Component/Component.hpp"
#include "Collider.hpp"
#include "Maths/Structs/Chain.hpp"
#include "Maths/Structs/Circle.hpp"
#include "Maths/Structs/Polygon.hpp"
#include "Maths/Structs/Rectangle.hpp"

namespace fro
{
	class Rigidbody final : public Component
	{
	public:
		class Implementation;

		enum class Type
		{
			STATIC,
			KINEMATIC,
			DYNAMIC
		};

		FRO_API Rigidbody();
		FRO_API Rigidbody(Rigidbody const& other);
		FRO_API Rigidbody(Rigidbody&& other) noexcept;

		FRO_API virtual ~Rigidbody() override;

		FRO_API Rigidbody& operator=(Rigidbody const& other);
		FRO_API Rigidbody& operator=(Rigidbody&& other) noexcept;

		FRO_API FRO_NODISCARD Implementation& getImplementation() const;

		FRO_API void setType(Type const type);

		FRO_API Collider& addCollider();

	private:
		std::unique_ptr<Implementation> mImplementation;
		std::vector<Collider> mColliders{};
	};
}

#endif