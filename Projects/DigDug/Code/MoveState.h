#if not defined fro_MOVE_STATE_H
#define fro_MOVE_STATE_H

#include "AudioService.h"
#include "GameObject.h"
#include "GridMovement.h"
#include "State.h"
#include "Scene.h"
#include "ServiceLocator.hpp"
#include "SpriteAnimator.h"

namespace fro
{
	class GameObject;

	class MoveState final : public State
	{
	public:
		MoveState(Reference<GameObject> const parentingGameObject);

		virtual ~MoveState() override = default;

		virtual Reference<State> update(float const deltaSeconds) override;
		virtual void enter(Reference<State> const) override;
		virtual void exit(Reference<State> const) override;

	private:
		MoveState(MoveState const&) = delete;
		MoveState(MoveState&&) noexcept = delete;

		MoveState& operator=(MoveState const&) = delete;
		MoveState& operator=(MoveState&&) noexcept = delete;

		AudioService& m_AudioService{ ServiceLocator<AudioService>::getInstance().getService() };

		Reference<SpriteAnimator> m_SpriteAnimator{
			parentingGameObject.get().forceGetComponent<SpriteAnimator>() };

		Reference<GridMovement> m_GridMovement{
			parentingGameObject.get().forceGetComponent<GridMovement>() };

		Reference<GameObject> m_Pump{ parentingGameObject.get().getParentingScene().get().forceGetGameObject("pump") };
	};
}

#endif