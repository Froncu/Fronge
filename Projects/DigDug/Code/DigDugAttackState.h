#if not defined fro_DIG_DUG_ATTACK_STATE_H
#define fro_DIG_DUG_ATTACK_STATE_H

#include "AudioService.h"
#include "State.h"
#include "ServiceLocator.hpp"

namespace fro
{
	class RigidBody;
	class SpriteAnimator;
	class GameObject;

	class DigDugAttackState final : public State
	{
	public:
		DigDugAttackState(Reference<GameObject> const parentingGameObject);

		virtual ~DigDugAttackState() override = default;

		virtual void awake() override;

		virtual Reference<State> fixedUpdate(float const) override;
		virtual void enter(Reference<State> const oldState) override;
		virtual void exit(Reference<State> const newState) override;

	private:
		DigDugAttackState(DigDugAttackState const&) = delete;
		DigDugAttackState(DigDugAttackState&&) noexcept = delete;

		DigDugAttackState& operator=(DigDugAttackState const&) = delete;
		DigDugAttackState& operator=(DigDugAttackState&&) noexcept = delete;

		AudioService& m_AudioService{ ServiceLocator<AudioService>::getInstance().getService() };

		Reference<RigidBody> m_RigidBody{};

		Reference<SpriteAnimator> m_SpriteAnimator{};

		Reference<GameObject> m_Pump{};
		Reference<SpriteAnimator> m_PumpSpriteAnimator{};

		Reference<GameObject> m_PumpHitBox{};
	};
}

#endif