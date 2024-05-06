#if not defined fro_TRANSFORM_H
#define fro_TRANSFORM_H

#include "Component.h"
#include "TransformationMatrix2D.h"

namespace fro
{
	class Transform final : public Component
	{
	public:
		Transform(GameObject const& parentingGameObject);

		virtual ~Transform() override = default;

		void localTransform(TransformationMatrix2D const& transformation);
		void localTranslate(glm::vec2 const& translation);
		void localRotate(float const rotation);
		void localScale(glm::vec2 const& scale);
		void worldTransform(TransformationMatrix2D const& transformation);
		void worldTranslate(glm::vec2 const& translation);
		void worldRotate(float const rotation);
		void worldScale(glm::vec2 const& scale);

		void setLocalTransformation(TransformationMatrix2D const& transformation);
		void setLocalTranslation(glm::vec2 const& translation);
		void setLocalRotation(float const rotation);
		void setLocalScale(glm::vec2 const& scale);
		void setWorldTransformDirty();
		void setWorldTransformation(TransformationMatrix2D const& transformation);
		void setWorldTranslation(glm::vec2 const& translation);
		void setWorldRotation(float const rotation);
		void setWorldScale(glm::vec2 const& scale);

		fro_NODISCARD TransformationMatrix2D const& getLocalTransform();
		fro_NODISCARD TransformationMatrix2D const& getWorldTransform();

	private:
		Transform(Transform const&) = delete;
		Transform(Transform&&) noexcept = delete;

		Transform& operator=(Transform const&) = delete;
		Transform& operator=(Transform&&) noexcept = delete;

		void calculateLocalTransform();
		void calculateWorldTransform();

		TransformationMatrix2D m_LocalTransform{};
		TransformationMatrix2D m_WorldTransform{};

		bool m_IsWorldTransformDirty{};
	};
}

#endif