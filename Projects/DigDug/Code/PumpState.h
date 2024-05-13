#if not defined fro_PUMP_STATE_H
#define fro_PUMP_STATE_H

#include "Component.h"
#include "State.h"

namespace fro
{
	class GameObject;

	class PumpState final : public Component, public State
	{
	public:
		PumpState(GameObject const& parentingGameObject);

		virtual ~PumpState() override = default;

		virtual std::unique_ptr<State> update(float const deltaSeconds) override;
		virtual void enter(std::unique_ptr<State> const& pOldState) override;
		virtual void exit(std::unique_ptr<State> const& pNewState) override;

	private:
		PumpState(PumpState const&) = delete;
		PumpState(PumpState&&) noexcept = delete;

		PumpState& operator=(PumpState const&) = delete;
		PumpState& operator=(PumpState&&) noexcept = delete;

		float m_ElapsedSecondsWithoutInput{};
	};
}

#endif