#if not defined RIGIDBODY_HPP
#define RIGIDBODY_HPP

#include "froch.hpp"

#include "ECS/Components/Component/Component.hpp"
#include "Events/Systems/EventListener.hpp"
#include "Maths/Structs/Rectangle.hpp"
#include "Maths/Structs/Vector2.hpp"

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

	private:
		std::unique_ptr<Implementation> mImplementation;
	};
}

#endif