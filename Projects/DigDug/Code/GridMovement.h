#if not defined fro_GRID_MOVEMENT_H
#define fro_GRID_MOVEMENT_H

#include "Behaviour.h"
#include "Event.hpp"

#include <vec2.hpp>

namespace fro
{
	class GridMovement final : public Behaviour
	{
	public:
		GridMovement(Reference<GameObject> const parentingGameObject);

		virtual ~GridMovement() override = default;

		virtual void update(float const deltaSeconds) override;

		void setMoveDirection(glm::vec2 const& direction);
		void setMoveSpeed(float const speed);

		Event<glm::vec2, glm::vec2> correctedMoveDirectionChanged{};

	private:
		GridMovement(GridMovement const&) = delete;
		GridMovement(GridMovement&&) noexcept = delete;

		GridMovement& operator=(GridMovement const&) = delete;
		GridMovement& operator=(GridMovement&&) noexcept = delete;

		glm::vec2 m_PreviousCorrectedMoveDirection{};

		glm::vec2 m_MoveDirection{};
		float m_MoveSpeed{ 32.0f };

		int m_CellsX{ 10 };
		int m_CellsY{ 8 };
		int m_CellSizeX{ 16 };
		int m_CellSizeY{ 16 };
	};
}

#endif