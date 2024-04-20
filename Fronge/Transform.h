#pragma once

#include "Component.h"
#include "Defines.hpp"

#include <glm.hpp>
#include <vec2.hpp>

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

		fro_NODISCARD_GETTER const glm::vec2& getLocalPosition();
		fro_NODISCARD_GETTER const glm::vec2& getWorldPosition();

		fro_NODISCARD_GETTER bool isLocalPositionDirty() const;
		fro_NODISCARD_GETTER bool isWorldPositionDirty() const;

	private:
		void calculateLocalPosition();
		void calculateWorldPosition();

		void setLocalPositionDirty();
		void setWorldPositionDirty();

		fro_NODISCARD_GETTER glm::vec2& getLocalPositionInternal();
		fro_NODISCARD_GETTER glm::vec2& getWorldPositionInternal();

		bool m_IsLocalPositionDirty{};
		bool m_IsWorldPositionDirty{};

		glm::vec2 m_LocalPosition{ 0.0f, 0.0f };
		glm::vec2 m_WorldPosition{ 0.0f, 0.0f };
	};
}