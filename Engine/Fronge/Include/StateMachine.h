#if not defined fro_STATE_MACHINE_H
#define fro_STATE_MACHINE_H

#include "Behaviour.h"
#include "State.h"

#include <memory>

namespace fro
{
	class StateMachine final : public Behaviour
	{
	public:
		StateMachine(GameObject const& parentingGameObject);

		virtual ~StateMachine() override = default;

		virtual void update() override;

	private:
		StateMachine(StateMachine const&) = delete;
		StateMachine(StateMachine&&) noexcept = delete;

		StateMachine& operator=(StateMachine const&) = delete;
		StateMachine& operator=(StateMachine&&) noexcept = delete;

		std::unique_ptr<State> m_pCurrentState{};
	};
}

#endif