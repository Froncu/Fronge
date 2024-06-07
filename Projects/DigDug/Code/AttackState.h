#if not defined fro_ATTACK_STATE_H
#define fro_ATTACK_STATE_H

#include "AudioService.h"
#include "GameObject.h"
#include "RigidBody.h"
#include "State.h"
#include "ServiceLocator.hpp"
#include "SpriteAnimator.h"

namespace fro
{
	class GameObject;

	class AttackState final : public State
	{
	public:
		AttackState(Reference<GameObject> const parentingGameObject);

		virtual ~AttackState() override = default;

		virtual Reference<State> fixedUpdate(float const) override;
		virtual void enter(Reference<State> const oldState) override;
		virtual void exit(Reference<State> const newState) override;

	private:
		AttackState(AttackState const&) = delete;
		AttackState(AttackState&&) noexcept = delete;

		AttackState& operator=(AttackState const&) = delete;
		AttackState& operator=(AttackState&&) noexcept = delete;

		AudioService& m_AudioService{ ServiceLocator<AudioService>::getInstance().getService() };

		Reference<RigidBody> const m_RigidBody{
			parentingGameObject.get().forceGetComponent<RigidBody>() };

		Reference<SpriteAnimator> const m_SpriteAnimator{
			parentingGameObject.get().forceGetComponent<SpriteAnimator>() };

		Reference<GameObject> const m_Pump{ parentingGameObject.get().getGameObject("pump") };
		Reference<SpriteAnimator> const m_PumpSpriteAnimator{ m_Pump.get().forceGetComponent<fro::SpriteAnimator>() };

		Reference<GameObject> const m_PumpHitBox{ parentingGameObject.get().forceGetGameObject("pumpHitBox") };
	};
}

#endif