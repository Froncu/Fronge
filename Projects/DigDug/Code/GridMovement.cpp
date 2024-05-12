#include "GridMovement.h"

#include "GameObject.h"
#include "InputManager.h"

#include <glm.hpp>

#pragma region Constructors/Destructor
fro::GridMovement::GridMovement(GameObject const& parentingGameObject)
	: Behaviour(parentingGameObject)
{
}
#pragma endregion Constructors/Destructor



#pragma region PublicMethods
void fro::GridMovement::update(float const deltaSeconds)
{
	glm::vec2 const inputAxis2D{ InputManager::getInstance().getActionStrengthAxis2D(m_aActions[0], m_aActions[1], m_aActions[2], m_aActions[3]) };
	if (not inputAxis2D.x and not inputAxis2D.y)
		return;

	glm::vec2 correctedMoveDirection{};

	int const halfCellSizeY{ m_CellSizeY / 2 };
	int const halfCellSizeX{ m_CellSizeX / 2 };
	glm::ivec2 const worldPosition{ getParentingGameObject().getComponent<Transform>()->getWorldTransform().getTranslation() };

	if (std::fabs(inputAxis2D.y) > std::fabs(inputAxis2D.x))
	{
		bool const isAtMaximaY{ worldPosition.y >= m_CellSizeY * m_CellsY - halfCellSizeY };
		bool const isAtMinimaY{ worldPosition.y <= halfCellSizeY };

		if ((isAtMaximaY and inputAxis2D.y < 0.0f) or
			(isAtMinimaY and inputAxis2D.y > 0.0f))
			return;

		if (int const offset{ worldPosition.x % m_CellSizeX }; offset > halfCellSizeX)
			correctedMoveDirection.x = -1.0f;
		else if (offset < halfCellSizeX)
			correctedMoveDirection.x = 1.0f;
		else
			correctedMoveDirection.y = std::signbit(inputAxis2D.y) ? 1.0f : -1.0f;
	}
	else
	{
		bool const isAtMaximaX{ worldPosition.x >= m_CellSizeX * m_CellsX - halfCellSizeX };
		bool const isAtMinimaX{ worldPosition.x <= halfCellSizeX };

		if ((isAtMaximaX and inputAxis2D.x > 0.0f) or
			(isAtMinimaX and inputAxis2D.x < 0.0f))
			return;

		if (int const offset{ worldPosition.y % m_CellSizeY }; offset < halfCellSizeY)
			correctedMoveDirection.y = 1.0f;
		else if (offset > halfCellSizeY)
			correctedMoveDirection.y = -1.0f;
		else
			correctedMoveDirection.x = std::signbit(inputAxis2D.x) ? -1.0f : 1.0f;
	}

	getParentingGameObject().getComponent<Transform>()->localTranslate(deltaSeconds * m_MoveSpeed * correctedMoveDirection);
}

void fro::GridMovement::setMoveSpeed(float const moveSpeed)
{
	m_MoveSpeed = moveSpeed;
}

void fro::GridMovement::setActionNames(std::string_view const positiveActionNameX,
	std::string_view const negativeActionNameX,
	std::string_view const positiveActionNameY,
	std::string_view const negativeActionNameY)
{
	m_aActions[0] = positiveActionNameX;
	m_aActions[1] = negativeActionNameX;
	m_aActions[2] = positiveActionNameY;
	m_aActions[3] = negativeActionNameY;
}
#pragma endregion PublicMethods