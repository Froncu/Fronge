#pragma once

#include "Behaviour.h"

#include <GLM/vec2.hpp>

fro_GENERATED_BEHAVIOUR_BODY
(
	Rotator,
	
	public:
		void setCenter(const glm::vec2& center);
		void setRadius(float radius);
		void setPeriod(float period);

	private:
		glm::vec2 m_Center{};
		float m_Radius{ 50.0f };
		float m_Period{ 3.14f };
		float m_Angle{};
)