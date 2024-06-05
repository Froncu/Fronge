#include "GridMovement.h"

#include <glm.hpp>

#pragma region Constructors/Destructor
fro::GridMovement::GridMovement(Reference<GameObject> const parentingGameObject)
	: Behaviour(parentingGameObject)
{
	m_RigidBody.get().setType(RigidBody::Type::dynamicBody);
	m_RigidBody.get().setGravityScale(0.0f);
}
#pragma endregion Constructors/Destructor



#pragma region PublicMethods
void fro::GridMovement::fixedUpdate(float const)
{
	if (not m_MoveDirection.x and not m_MoveDirection.y)
	{
		m_RigidBody.get().setVelocity({});
		return;
	}

	glm::vec2 correctedMoveDirection{};

	int const halfCellSizeY{ m_CellSizeY / 2 };
	int const halfCellSizeX{ m_CellSizeX / 2 };
	glm::ivec2 const worldPosition{ m_ParentingGameObject.get().getWorldTransform().getTranslation() };

	if (std::abs(m_MoveDirection.y) > std::abs(m_MoveDirection.x))
	{
		bool const isAtMaximaY{ worldPosition.y >= m_CellSizeY * m_CellsY - halfCellSizeY };
		bool const isAtMinimaY{ worldPosition.y <= halfCellSizeY };

		if ((isAtMaximaY and m_MoveDirection.y < 0.0f) or
			(isAtMinimaY and m_MoveDirection.y > 0.0f))
		{
			m_RigidBody.get().setVelocity({});
			return;
		}

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
		{
			m_RigidBody.get().setVelocity({});
			return;
		}

		if (int const offset{ worldPosition.y % m_CellSizeY }; offset < halfCellSizeY)
			correctedMoveDirection.y = 1.0f;
		else if (offset > halfCellSizeY)
			correctedMoveDirection.y = -1.0f;
		else
			correctedMoveDirection.x = std::signbit(m_MoveDirection.x) ? -1.0f : 1.0f;
	}

	m_RigidBody.get().setVelocity(correctedMoveDirection * m_MoveSpeed);

	if (correctedMoveDirection.x)
		m_ParentingGameObject.get().setLocalScale({ correctedMoveDirection.x, 1.0f });

	if (m_PreviousCorrectedMoveDirection not_eq correctedMoveDirection)
	{
		correctedMoveDirectionChanged.notifySubscribers(
			m_PreviousCorrectedMoveDirection, correctedMoveDirection);

		m_PreviousCorrectedMoveDirection = correctedMoveDirection;
	}
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