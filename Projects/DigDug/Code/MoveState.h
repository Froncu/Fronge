#if not defined fro_MOVE_STATE_H
#define fro_MOVE_STATE_H

#include "State.h"

namespace fro
{
	class GameObject;

	class MoveState final : public State
	{
	public:
		MoveState(Reference<GameObject> const parentingGameObject);

		virtual ~MoveState() override = default;

		virtual Reference<State> update(float const deltaSeconds) override;
		virtual void enter(Reference<State> const) override;
		virtual void exit(Reference<State> const) override;

	private:
		MoveState(MoveState const&) = delete;
		MoveState(MoveState&&) noexcept = delete;

		MoveState& operator=(MoveState const&) = delete;
		MoveState& operator=(MoveState&&) noexcept = delete;
	};
}

#endif