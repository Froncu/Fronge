#if not defined fro_ROTATOR_H
#define fro_ROTATOR_H

#include "Behaviour.h"
#include "GameObject.h"

#include <vec2.hpp>

namespace fro
{
	class Rotator final : public Behaviour
	{
	public:
		Rotator(Reference<GameObject> const parentingGameObject);

		virtual ~Rotator() override = default;

		virtual void update(float const deltaSeconds) override;

		void setCenter(glm::vec2 const& center);
		void setRadius(float const radius);
		void setPeriod(float const period);

	private:
		Rotator(Rotator const&) = delete;
		Rotator(Rotator&&) noexcept = delete;

		Rotator& operator=(Rotator const&) = delete;
		Rotator& operator=(Rotator&&) noexcept = delete;

		glm::vec2 m_Center{};
		float m_Radius{ 50.0f };
		float m_Period{ 1 };
		float m_Angle{};
	};
}

#endif