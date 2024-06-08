#if not defined fro_DIG_DUG_MOVE_STATE_H
#define fro_DIG_DUG_MOVE_STATE_H

#include "AudioService.h"
#include "GameObject.h"
#include "GridMovement.h"
#include "RigidBody.h"
#include "State.h"
#include "Scene.h"
#include "ServiceLocator.hpp"
#include "SpriteAnimator.h"

namespace fro
{
	class GameObject;

	class DigDugMoveState final : public State
	{
	public:
		DigDugMoveState(Reference<GameObject> const parentingGameObject);

		virtual ~DigDugMoveState() override = default;

		virtual void awake() override;

		virtual Reference<State> update(float const deltaSeconds) override;
		virtual void enter(Reference<State> const) override;
		virtual void exit(Reference<State> const) override;

	private:
		DigDugMoveState(DigDugMoveState const&) = delete;
		DigDugMoveState(DigDugMoveState&&) noexcept = delete;

		DigDugMoveState& operator=(DigDugMoveState const&) = delete;
		DigDugMoveState& operator=(DigDugMoveState&&) noexcept = delete;

		AudioService& m_AudioService{ ServiceLocator<AudioService>::getInstance().getService() };

		Reference<RigidBody> m_RigidBody{};

		Reference<SpriteAnimator> m_SpriteAnimator{};

		Reference<GridMovement> m_GridMovement{};

		Reference<GameObject> m_Pump{};
	};
}

#endif