#if not defined fro_IDLE_STATE_H
#define fro_IDLE_STATE_H

#include "Component.h"
#include "GameObject.h"
#include "GridMovement.h"
#include "SpriteAnimator.h"
#include "Scene.h"
#include "State.h"

namespace fro
{
	class IdleState final : public State
	{
	public:
		IdleState(Reference<GameObject> const parentingGameObject);

		virtual ~IdleState() override = default;

		virtual Reference<State> update(float const deltaSeconds) override;
		virtual void enter(Reference<State> const pOldState) override;

	private:
		IdleState(IdleState const&) = delete;
		IdleState(IdleState&&) noexcept = delete;

		IdleState& operator=(IdleState const&) = delete;
		IdleState& operator=(IdleState&&) noexcept = delete;

		Reference<SpriteAnimator> m_SpriteAnimator{
			parentingGameObject.get().forceGetComponent<SpriteAnimator>() };
		Reference<GridMovement> m_GridMovement{
			parentingGameObject.get().getComponent<GridMovement>() };

		Reference<GameObject> m_Pump{ parentingGameObject.get().getParentingScene().get().forceGetGameObject("pump") };
	};
}

#endif