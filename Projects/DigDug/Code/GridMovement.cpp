#include "GridMovement.h"

#include "GameObject.h"

#include <glm.hpp>

#pragma region Constructors/Destructor
fro::GridMovement::GridMovement(GameObject& parentingGameObject)
	: Behaviour(parentingGameObject)
{
}
#pragma endregion Constructors/Destructor



#pragma region PublicMethods
void fro::GridMovement::update(float const deltaSeconds)
{
	if (not m_MoveDirection.x and not m_MoveDirection.y)
		return;

	glm::vec2 correctedMoveDirection{};

	int const halfCellSizeY{ m_CellSizeY / 2 };
	int const halfCellSizeX{ m_CellSizeX / 2 };
	glm::ivec2 const worldPosition{ getParentingGameObject().getComponent<Transform>()->getWorldTransform().getTranslation() };

	if (std::fabs(m_MoveDirection.y) > std::fabs(m_MoveDirection.x))
	{
		bool const isAtMaximaY{ worldPosition.y >= m_CellSizeY * m_CellsY - halfCellSizeY };
		bool const isAtMinimaY{ worldPosition.y <= halfCellSizeY };

		if ((isAtMaximaY and m_MoveDirection.y < 0.0f) or
			(isAtMinimaY and m_MoveDirection.y > 0.0f))
			return;

		if (int const offset{ worldPosition.x % m_CellSizeX }; offset > halfCellSizeX)
			correctedMoveDirection.x = -1.0f;
		else if (offset < halfCellSizeX)
			correctedMoveDirection.x = 1.0f;
		else
			correctedMoveDirection.y = std::signbit(m_MoveDirection.y) ? 1.0f : -1.0f;
	}
	else
	{
		bool const isAtMaximaX{ worldPosition.x >= m_CellSizeX * m_CellsX - halfCellSizeX };
		bool const isAtMinimaX{ worldPosition.x <= halfCellSizeX };

		if ((isAtMaximaX and m_MoveDirection.x > 0.0f) or
			(isAtMinimaX and m_MoveDirection.x < 0.0f))
			return;

		if (int const offset{ worldPosition.y % m_CellSizeY }; offset < halfCellSizeY)
			correctedMoveDirection.y = 1.0f;
		else if (offset > halfCellSizeY)
			correctedMoveDirection.y = -1.0f;
		else
			correctedMoveDirection.x = std::signbit(m_MoveDirection.x) ? -1.0f : 1.0f;
	}

	Transform& transform{ *getParentingGameObject().getComponent<Transform>() };
	transform.localTranslate(deltaSeconds * m_MoveSpeed * correctedMoveDirection);
}

void fro::GridMovement::setMoveDirection(glm::vec2 const& direction)
{
	m_MoveDirection = direction;
}

void fro::GridMovement::setMoveSpeed(float const moveSpeed)
{
	m_MoveSpeed = moveSpeed;
}
#pragma endregion PublicMethods