#if not defined fro_STATE_MACHINE_H
#define fro_STATE_MACHINE_H

#include "Behaviour.h"

#include <memory>

namespace fro
{
	class State;

	class StateMachine final : public Behaviour
	{
	public:
		StateMachine(Reference<GameObject> const parentingGameObject);

		virtual ~StateMachine() override = default;

		virtual void fixedUpdate(float const fixedDeltaSeconds) override;
		virtual void update(float const deltaSeconds) override;
		virtual void lateUpdate(float const deltaSeconds) override;

		void setCurrentState(Reference<State> const newState);

	private:
		StateMachine(StateMachine const&) = delete;
		StateMachine(StateMachine&&) noexcept = delete;

		StateMachine& operator=(StateMachine const&) = delete;
		StateMachine& operator=(StateMachine&&) noexcept = delete;

		void updateState(Reference<State>&& newState);

		Reference<State> m_CurrentState{};
	};
}

#endif