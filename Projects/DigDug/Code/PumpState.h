#if not defined fro_PUMP_STATE_H
#define fro_PUMP_STATE_H

#include "AudioService.h"
#include "GameObject.h"
#include "RigidBody.h"
#include "State.h"
#include "ServiceLocator.hpp"
#include "SpriteAnimator.h"

namespace fro
{
	class GameObject;

	class PumpState final : public State
	{
	public:
		PumpState(Reference<GameObject> const parentingGameObject);

		virtual ~PumpState() override = default;

		virtual Reference<State> update(float const deltaSeconds) override;
		virtual void enter(Reference<State> const) override;
		virtual void exit(Reference<State> const) override;

	private:
		PumpState(PumpState const&) = delete;
		PumpState(PumpState&&) noexcept = delete;

		PumpState& operator=(PumpState const&) = delete;
		PumpState& operator=(PumpState&&) noexcept = delete;

		Reference<RigidBody> const m_RigidBody{
			parentingGameObject.get().forceGetComponent<RigidBody>() };

		Reference<SpriteAnimator> m_SpriteAnimator
		{
			parentingGameObject.get().forceGetComponent<SpriteAnimator>()
		};

		AudioService& m_AudioService{ ServiceLocator<AudioService>::getInstance().getService() };

		float const m_CooldownSeconds{ 0.5f };
		float const m_IdleTimeoutSeconds{ 0.6f };

		float m_ElapsedSecondsWithoutInput{};
		float m_RemainingCooldownSeconds{};
	};
}

#endif