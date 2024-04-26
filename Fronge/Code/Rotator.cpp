#include "Rotator.h"

#include "Timer.h"
#include "GameObject.h"
#include "Transform.h"

#include <gtc/constants.hpp>
#include <glm.hpp>

#pragma region Constructors/Destructor
fro::Rotator::Rotator(const GameObject& parentingGameObject) :
	Behaviour(parentingGameObject)
{
}
#pragma endregion Constructors/Destructor



#pragma region PublicMethods
void fro::Rotator::update()
{
	constexpr float twoPi{ 2 * glm::pi<float>() };

	m_Angle += Timer::getInstance().getDeltaSeconds() * twoPi / m_Period;

	glm::mod(m_Angle, twoPi);

	getParentingGameObject().getComponent<Transform>()->setLocalPosition(m_Center + m_Radius * glm::vec2(glm::cos(m_Angle), glm::sin(m_Angle)));
}

void fro::Rotator::setCenter(const glm::vec2& center)
{
	m_Center = center;
}

void fro::Rotator::setRadius(float radius)
{
	m_Radius = radius;
}

void fro::Rotator::setPeriod(float period)
{
	m_Period = period;
}
#pragma endregion PublicMethods