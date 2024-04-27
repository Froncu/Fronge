#if not defined fro_TRANSFORM_H
#define fro_TRANSFORM_H

#include "Component.h"

#include <vec2.hpp>

namespace fro
{
	class Transform final : public Component
	{
	public:
		Transform(GameObject const& parentingGameObject);

		virtual ~Transform() override = default;

		void localTranslate(glm::vec2 const& localTranslator);
		void worldTranslate(glm::vec2 const& worldTranslator);

		void setLocalPosition(glm::vec2 const& localPosition);
		void setWorldPosition(glm::vec2 const& worldPosition);

		fro_NODISCARD glm::vec2 const& getLocalPosition();
		fro_NODISCARD glm::vec2 const& getWorldPosition();

		fro_NODISCARD bool isLocalPositionDirty() const;
		fro_NODISCARD bool isWorldPositionDirty() const;

	private:
		Transform(Transform const&) = delete;
		Transform(Transform&&) noexcept = delete;

		Transform& operator=(Transform const&) = delete;
		Transform& operator=(Transform&&) noexcept = delete;

		void calculateLocalPosition();
		void calculateWorldPosition();

		void setLocalPositionDirty();
		void setWorldPositionDirty();

		fro_NODISCARD glm::vec2& getLocalPositionInternal();
		fro_NODISCARD glm::vec2& getWorldPositionInternal();

		bool m_IsLocalPositionDirty{};
		bool m_IsWorldPositionDirty{};

		glm::vec2 m_LocalPosition{ 0.0f, 0.0f };
		glm::vec2 m_WorldPosition{ 0.0f, 0.0f };
	};
}

#endif