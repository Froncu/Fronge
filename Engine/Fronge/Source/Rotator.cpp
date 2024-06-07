#include "Rotator.h"

#include <glm.hpp>
#include <gtc/constants.hpp>

#pragma region Constructors/Destructor
fro::Rotator::Rotator(Reference<GameObject> const parentingGameObject)
	: Behaviour(parentingGameObject)
{
}
#pragma endregion Constructors/Destructor



#pragma region PublicMethods
void fro::Rotator::update(float const deltaSeconds)
{
	float constexpr twoPi{ 2 * glm::pi<float>() };

	m_Angle += deltaSeconds * twoPi / m_Period;

	m_Angle = glm::mod(m_Angle, twoPi);

	parentingGameObject.get().setWorldTranslation(m_Center + m_Radius * glm::vec2(glm::cos(m_Angle), glm::sin(m_Angle)));
	parentingGameObject.get().setWorldRotation(m_Angle);
}

void fro::Rotator::setCenter(glm::vec2 const& center)
{
	m_Center = center;
}

void fro::Rotator::setRadius(float const radius)
{
	m_Radius = radius;
}

void fro::Rotator::setPeriod(float const period)
{
	m_Period = period;
}
#pragma endregion PublicMethods