#if not defined fro_MOVE_STATE_H
#define fro_MOVE_STATE_H

#include "Component.h"
#include "State.h"

namespace fro
{
	class GameObject;

	class MoveState final : public Component, public State
	{
	public:
		MoveState(Reference<GameObject> const parentingGameObject);

		virtual ~MoveState() override = default;

		virtual std::unique_ptr<State> update(float const deltaSeconds) override;
		virtual void enter(std::unique_ptr<State> const& pOldState) override;
		virtual void exit(std::unique_ptr<State> const& pNewState) override;

	private:
		MoveState(MoveState const&) = delete;
		MoveState(MoveState&&) noexcept = delete;

		MoveState& operator=(MoveState const&) = delete;
		MoveState& operator=(MoveState&&) noexcept = delete;
	};
}

#endif