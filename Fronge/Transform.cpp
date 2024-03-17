#include "Transform.h"

#include "GameObject.h"

#pragma region Constructors/Destructor
fro_GENERATED_COMPONENT_CONSTRUCTOR(Transform)
fro_GENERATED_COMPONENT_DESTRUCTOR(Transform)
#pragma endregion Constructors/Destructor



#pragma region PublicMethods
void fro::Transform::localTranslate(const glm::vec2& translator)
{
	m_LocalPosition += translator;
}

void fro::Transform::setLocalPosition(const glm::vec2& position)
{
	m_LocalPosition = position;
}

const glm::vec2& fro::Transform::getLocalPosition() const
{
	return m_LocalPosition;
}
#pragma endregion PublicMethods