#if not defined TRANSFORM_HPP
#define TRANSFORM_HPP

#include "Core.hpp"
#include "ECS/Components/Component/Component.hpp"
#include "ECS/Entity/Entity.hpp"
#include "Maths/Structs/TransformMatrix/TransformMatrix3x3.hpp"
#include "Reference/Reference.hpp"

namespace fro
{
	class Transform final : public Component
	{
	public:
		Transform() = default;
		Transform(Transform const&) = default;
		Transform(Transform&&) noexcept = default;

		FRO_API virtual ~Transform() override;

		Transform& operator=(Transform const&) = default;
		Transform& operator=(Transform&&) noexcept = default;

		FRO_API bool setParent(Transform* const parent, bool keepWorldTransform = true);

		FRO_API FRO_NODISCARD bool isParenting(Transform const& transform) const;
		
		FRO_API void setLocalTranslation(Vector2<double> const translation);
		FRO_API void setLocalRotation(double const rotation);
		FRO_API void setLocalScale(Vector2<double> const scale);
		
		FRO_API void localTranslate(Vector2<double> const translation);
		FRO_API void localRotate(double const rotation);
		FRO_API void localScale(Vector2<double> const scale);
		
		FRO_API void setWorldTranslation(Vector2<double> const translation);
		FRO_API void setWorldRotation(double const rotation);
		FRO_API void setWorldScale(Vector2<double> const scale);
		
		FRO_API void worldTranslate(Vector2<double> const translation);
		FRO_API void worldRotate(double const rotation);
		FRO_API void worldScale(Vector2<double> const scale);

		FRO_API FRO_NODISCARD TransformMatrix3x3 const& local() const;
		FRO_API FRO_NODISCARD TransformMatrix3x3 const& world() const;

	private:
		void setWorldTransformDirty();
		FRO_NODISCARD TransformMatrix3x3& getWorldTransform() const;
		void calculateWorldTransform() const;
		void calculateLocalTransform();

		TransformMatrix3x3 mLocalTransform{};
		mutable TransformMatrix3x3 mWorldTransform{};

		std::vector<Reference<Transform>> mChildren{};
		Reference<Transform> mParent{};

		mutable bool mIsWorldTransformDirty{};
	};
}

#endif