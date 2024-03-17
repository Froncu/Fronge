#pragma once

#include "Component.h"

#include <GLM/glm.hpp>
#include <GLM/vec2.hpp>

fro_GENERATED_COMPONENT_BODY
(
	Transform,
	public:
		void localTranslate(const glm::vec2& translator);

		void setLocalPosition(const glm::vec2& position);

		const glm::vec2& getLocalPosition();

	private:
		glm::vec2 m_LocalPosition{ 0.0f, 0.0f };
)