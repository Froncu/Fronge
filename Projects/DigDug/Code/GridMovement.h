#if not defined fro_GRID_MOVEMENT_H
#define fro_GRID_MOVEMENT_H

#include "Behaviour.h"

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

	private:
		GridMovement(GridMovement const&) = delete;
		GridMovement(GridMovement&&) noexcept = delete;

		GridMovement& operator=(GridMovement const&) = delete;
		GridMovement& operator=(GridMovement&&) noexcept = delete;

		glm::vec2 m_MoveDirection{};
		float m_MoveSpeed{ 32.0f };
		float m_ScalarX{ 1.0f };

		int m_CellsX{ 10 };
		int m_CellsY{ 8 };
		int m_CellSizeX{ 16 };
		int m_CellSizeY{ 16 };
	};
}

#endif