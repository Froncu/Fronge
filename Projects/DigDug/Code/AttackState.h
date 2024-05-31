#if not defined fro_ATTACK_STATE_H
#define fro_ATTACK_STATE_H

#include "Component.h"
#include "State.h"

namespace fro
{
	class GameObject;

	class AttackState final : public Component, public State
	{
	public:
		AttackState(Reference<GameObject> const parentingGameObject);

		virtual ~AttackState() override = default;

		virtual std::unique_ptr<State> update(float const deltaSeconds) override;
		virtual void enter(std::unique_ptr<State> const& pOldState) override;
		virtual void exit(std::unique_ptr<State> const& pNewState) override;

	private:
		AttackState(AttackState const&) = delete;
		AttackState(AttackState&&) noexcept = delete;

		AttackState& operator=(AttackState const&) = delete;
		AttackState& operator=(AttackState&&) noexcept = delete;

		float m_ElapsedSeconds{};
	};
}

#endif