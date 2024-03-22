#pragma once

#include "Component.h"

#include <GLM/glm.hpp>
#include <GLM/vec2.hpp>

namespace fro
{
	class Transform final : public Component
	{
		fro_GENERATED_COMPONENT_BODY(Transform)

	public:
		void localTranslate(const glm::vec2& localTranslator);
		void worldTranslate(const glm::vec2& worldTranslator);

		void setLocalPosition(const glm::vec2& localPosition);
		void setWorldPosition(const glm::vec2& worldPosition);

		const glm::vec2& getLocalPosition();
		const glm::vec2& getWorldPosition();

	private:
		void calculateLocalPosition();
		void calculateWorldPosition();

		void setLocalPositionDirty();
		void setWorldPositionDirty();

		glm::vec2& getLocalPositionInternal();
		glm::vec2& getWorldPositionInternal();

		bool m_IsLocalPositionDirty{};
		bool m_IsWorldPositionDirty{};

		glm::vec2 m_LocalPosition{ 0.0f, 0.0f };
		glm::vec2 m_WorldPosition{ 0.0f, 0.0f };
	};
}