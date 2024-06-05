#include "MovementRotation.h"

#include "GameObject.h"

#pragma region Constructors/Destructor
fro::MovementRotation::MovementRotation(Reference<GameObject> const parentingGameObject)
	: Component(parentingGameObject)
{
}
#pragma endregion Constructors/Destructor



#pragma region PublicMethods
void fro::MovementRotation::onCorrectedMoveDirectionChanged(
	glm::vec2 const previous, glm::vec2 const current)
{
	if (previous.y and current.x)
	{
		m_ParentingGameObject.get().setLocalRotation(0.0f);
		return;
	}

	if (previous.x and current.y)
	{
		if (previous.x > 0.0f)
			m_ParentingGameObject.get().setLocalRotation(glm::radians(
				current.y > 0.0f ? 90.0f : -90.0f));
		else
			m_ParentingGameObject.get().setLocalRotation(glm::radians(
				current.y > 0.0f ? -90.0f : 90.0f));
	}
	else
		m_ParentingGameObject.get().localScale({ previous.y not_eq current.y ? -1.0f : 1.0f, 1.0f });
}
#pragma endregion PublicMethods