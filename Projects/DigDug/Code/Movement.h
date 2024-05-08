#if not defined fro_MOVE_COMMAND_H
#define fro_MOVE_COMMAND_H

#include "Behaviour.h"

#include <vec2.hpp>

namespace fro
{
	class Movement final : public Behaviour
	{
	public:
		Movement(GameObject const& parentingGameObject);

		virtual ~Movement() override = default;

		virtual void update() override;

		void setMoveSpeed(float const moveSpeed);

	private:
		Movement(Movement const&) = delete;
		Movement(Movement&&) noexcept = delete;

		Movement& operator=(Movement const&) = delete;
		Movement& operator=(Movement&&) noexcept = delete;

		float m_MoveSpeed{ 32.0f };
	};
}

#endif