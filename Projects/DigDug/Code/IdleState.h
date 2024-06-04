#if not defined fro_IDLE_STATE_H
#define fro_IDLE_STATE_H

#include "Component.h"
#include "State.h"

namespace fro
{
	class GameObject;

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
	};
}

#endif