#if not defined fro_PUMP_STATE_H
#define fro_PUMP_STATE_H

#include "State.h"

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

		float m_ElapsedSecondsWithoutInput{};
	};
}

#endif