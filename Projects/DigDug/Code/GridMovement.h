#if not defined fro_GRID_MOVEMENT_H
#define fro_GRID_MOVEMENT_H

#include "Behaviour.h"

#include <vec2.hpp>

namespace fro
{
	class GridMovement final : public Behaviour
	{
	public:
		GridMovement(GameObject const& parentingGameObject);

		virtual ~GridMovement() override = default;

		virtual void update() override;

		void setMoveSpeed(float const moveSpeed);

	private:
		GridMovement(GridMovement const&) = delete;
		GridMovement(GridMovement&&) noexcept = delete;

		GridMovement& operator=(GridMovement const&) = delete;
		GridMovement& operator=(GridMovement&&) noexcept = delete;

		int m_CellsX{ 10 };
		int m_CellsY{ 8 };
		int m_CellSizeX{ 16 };
		int m_CellSizeY{ 16 };
		float m_MoveSpeed{ 32.0f };
	};
}

#endif