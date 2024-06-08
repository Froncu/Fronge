#if not defined fro_DIG_DUG_IDLE_STATE_H
#define fro_DIG_DUG_IDLE_STATE_H

#include "Component.h"
#include "GameObject.h"
#include "GridMovement.h"
#include "RigidBody.h"
#include "SpriteAnimator.h"
#include "Scene.h"
#include "State.h"

namespace fro
{
	class DigDugIdleState final : public State
	{
	public:
		DigDugIdleState(Reference<GameObject> const parentingGameObject);

		virtual ~DigDugIdleState() override = default;

		virtual void awake() override;

		virtual Reference<State> update(float const deltaSeconds) override;
		virtual void enter(Reference<State> const pOldState) override;

	private:
		DigDugIdleState(DigDugIdleState const&) = delete;
		DigDugIdleState(DigDugIdleState&&) noexcept = delete;

		DigDugIdleState& operator=(DigDugIdleState const&) = delete;
		DigDugIdleState& operator=(DigDugIdleState&&) noexcept = delete;

		Reference<RigidBody> m_RigidBody{};

		Reference<SpriteAnimator> m_SpriteAnimator{};
		Reference<GridMovement> m_GridMovement{};

		Reference<GameObject> m_Pump{};
	};
}

#endif