#if not defined fro_DIG_DUG_DEAD_STATE_H
#define fro_DIG_DUG_DEAD_STATE_H

#include "AudioService.h"
#include "ServiceLocator.hpp"
#include "State.h"

namespace fro
{
	class GameObject;
	class SpriteAnimator;

	class DigDugDeadState final : public State
	{
	public:
		DigDugDeadState(Reference<GameObject> const parentinGameObject);

		virtual ~DigDugDeadState() override = default;

		virtual void awake() override;

		virtual Reference<State> fixedUpdate(float const);

		virtual void enter(Reference<State> const) override;

	private:
		DigDugDeadState(DigDugDeadState const&) = delete;
		DigDugDeadState(DigDugDeadState&&) noexcept = delete;

		DigDugDeadState& operator=(DigDugDeadState const&) = delete;
		DigDugDeadState& operator=(DigDugDeadState&&) noexcept = delete;

		AudioService& m_AudioService{ ServiceLocator<AudioService>::getInstance().getService() };

		Reference<SpriteAnimator>m_SpriteAnimator{};

		Reference<GameObject> m_Pump{};
	};
}

#endif