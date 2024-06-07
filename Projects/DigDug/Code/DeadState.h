#if not defined fro_DEAD_STATE_H
#define fro_DEAD_STATE_H

#include "AudioService.h"
#include "ServiceLocator.hpp"
#include "SpriteAnimator.h"
#include "State.h"

namespace fro
{
	class DeadState final : public State
	{
	public:
		DeadState(Reference<GameObject> const parentinGameObject);

		virtual ~DeadState() override = default;

		virtual Reference<State> fixedUpdate(float const);

		virtual void enter(Reference<State> const) override;

	private:
		DeadState(DeadState const&) = delete;
		DeadState(DeadState&&) noexcept = delete;

		DeadState& operator=(DeadState const&) = delete;
		DeadState& operator=(DeadState&&) noexcept = delete;

		AudioService& m_AudioService{ ServiceLocator<AudioService>::getInstance().getService() };

		Reference<SpriteAnimator> const m_SpriteAnimator{
			parentingGameObject.get().forceGetComponent<SpriteAnimator>() };

		Reference<GameObject> const m_Pump{ parentingGameObject.get().getGameObject("pump") };
	};
}

#endif