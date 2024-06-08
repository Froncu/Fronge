#if not defined fro_FYGAR_ATTACK_STATE_H
#define fro_FYGAR_ATTACK_STATE_H

#include "State.h"

namespace fro
{
	class FygarAttackState final : public State
	{
	public:
		FygarAttackState(Reference<GameObject> const parentingGameObject);

		virtual ~FygarAttackState() override = default;

		virtual Reference<State> update(float const deltaSeconds) override;

	private:
		FygarAttackState(FygarAttackState const&) = delete;
		FygarAttackState(FygarAttackState&&) noexcept = delete;

		FygarAttackState& operator=(FygarAttackState const&) = delete;
		FygarAttackState& operator=(FygarAttackState&&) noexcept = delete;

		float const m_ChargeTimeSeconds{ 1.0f };
		float m_ElapsedSeconds{};
	};
}

#endif