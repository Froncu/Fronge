#if not defined fro_FPS_COUNTER_H
#define fro_FPS_COUNTER_H

#include "Behaviour.h"

namespace fro
{
	class FPSCounter final : public Behaviour
	{
	public:
		FPSCounter(GameObject const& parentingGameObject);

		virtual ~FPSCounter() override = default;

		virtual void update(float const deltaSeconds) override;

	private:
		FPSCounter(FPSCounter const&) = delete;
		FPSCounter(FPSCounter&&) noexcept = delete;

		FPSCounter& operator=(FPSCounter const&) = delete;
		FPSCounter& operator=(FPSCounter&&) noexcept = delete;

		int m_Ticks{};
		float m_ElapsedSeconds{};
	};
}

#endif