#if not defined fro_FYGAR_FIRE_STATE_H
#define fro_FYGAR_FIRE_STATE_H

#include "State.h"

namespace fro
{
	class FygarFireState final : public State
	{
	public:
		FygarFireState(Reference<GameObject> const parentingGameObject);

		virtual ~FygarFireState() override = default;

		virtual Reference<State> update(float const deltaSeconds) override;

	private:
		FygarFireState(FygarFireState const&) = delete;
		FygarFireState(FygarFireState&&) noexcept = delete;

		FygarFireState& operator=(FygarFireState const&) = delete;
		FygarFireState& operator=(FygarFireState&&) noexcept = delete;

		float const m_ChargeTimeSeconds{ 1.0f };
		float m_ElapsedSeconds{};
	};
}

#endif