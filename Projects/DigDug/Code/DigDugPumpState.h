#if not defined fro_DIG_DUG_PUMP_STATE_H
#define fro_DIG_DUG_PUMP_STATE_H

#include "AudioService.h"
#include "GameObject.h"
#include "RigidBody.h"
#include "State.h"
#include "ServiceLocator.hpp"
#include "SpriteAnimator.h"

namespace fro
{
	class GameObject;

	class DigDugPumpState final : public State
	{
		friend class DigDugAttackState;

	public:
		DigDugPumpState(Reference<GameObject> const parentingGameObject);

		virtual ~DigDugPumpState() override = default;

		virtual void awake() override;

		virtual Reference<State> update(float const deltaSeconds) override;
		virtual void enter(Reference<State> const) override;
		virtual void exit(Reference<State> const) override;

	private:
		DigDugPumpState(DigDugPumpState const&) = delete;
		DigDugPumpState(DigDugPumpState&&) noexcept = delete;

		DigDugPumpState& operator=(DigDugPumpState const&) = delete;
		DigDugPumpState& operator=(DigDugPumpState&&) noexcept = delete;

		Reference<GameObject> m_AttackedEnemy{};

		Reference<RigidBody> m_RigidBody{};

		Reference<SpriteAnimator> m_SpriteAnimator{};

		AudioService& m_AudioService{ ServiceLocator<AudioService>::getInstance().getService() };

		float const m_CooldownSeconds{ 0.5f };
		float const m_IdleTimeoutSeconds{ 0.6f };

		float m_ElapsedSecondsWithoutInput{};
		float m_RemainingCooldownSeconds{};
	};
}

#endif