#if not defined fro_ATTACK_STATE_H
#define fro_ATTACK_STATE_H

#include "State.h"

namespace fro
{
	class GameObject;

	class AttackState final : public State
	{
	public:
		AttackState(Reference<GameObject> const parentingGameObject);

		virtual ~AttackState() override = default;

		virtual Reference<State> update(float const deltaSeconds) override;
		virtual void enter(Reference<State> const oldState) override;
		virtual void exit(Reference<State> const newState) override;

	private:
		AttackState(AttackState const&) = delete;
		AttackState(AttackState&&) noexcept = delete;

		AttackState& operator=(AttackState const&) = delete;
		AttackState& operator=(AttackState&&) noexcept = delete;

		float m_ElapsedSeconds{};
	};
}

#endif