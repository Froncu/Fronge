#pragma once

#include "Component.h"

#include <vec2.hpp>

namespace fro
{
	class Transform final : public Component
	{
	public:
		Transform(const GameObject& parentingGameObject);

		virtual ~Transform() override = default;

		void localTranslate(const glm::vec2& localTranslator);
		void worldTranslate(const glm::vec2& worldTranslator);

		void setLocalPosition(const glm::vec2& localPosition);
		void setWorldPosition(const glm::vec2& worldPosition);

		fro_NODISCARD const glm::vec2& getLocalPosition();
		fro_NODISCARD const glm::vec2& getWorldPosition();

		fro_NODISCARD bool isLocalPositionDirty() const;
		fro_NODISCARD bool isWorldPositionDirty() const;

	private:
		Transform(const Transform&) = delete;
		Transform(Transform&&) noexcept = delete;

		Transform& operator=(const Transform&) = delete;
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