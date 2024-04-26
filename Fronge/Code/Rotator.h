#pragma once

#include "Behaviour.h"

#include <vec2.hpp>

namespace fro
{
	class Rotator final : public Behaviour
	{
	public:
		Rotator(const GameObject& parentingGameObject);

		virtual ~Rotator() override = default;

		virtual void update() override;

		void setCenter(const glm::vec2& center);
		void setRadius(float radius);
		void setPeriod(float period);

	private:
		Rotator(const Rotator&) = delete;
		Rotator(Rotator&&) noexcept = delete;

		Rotator& operator=(const Rotator&) = delete;
		Rotator& operator=(Rotator&&) noexcept = delete;

		glm::vec2 m_Center{};
		float m_Radius{ 50.0f };
		float m_Period{ 1 };
		float m_Angle{};
	};
}