#if not defined fro_FPS_COUNTER_H
#define fro_FPS_COUNTER_H

#include "Behaviour.h"

namespace fro
{
	class Text;

	class FPSCounter final : public Behaviour
	{
	public:
		FPSCounter(Reference<GameObject> const parentingGameObject);

		virtual ~FPSCounter() override = default;

		virtual void update(float const deltaSeconds) override;

	private:
		FPSCounter(FPSCounter const&) = delete;
		FPSCounter(FPSCounter&&) noexcept = delete;

		FPSCounter& operator=(FPSCounter const&) = delete;
		FPSCounter& operator=(FPSCounter&&) noexcept = delete;

		Reference<Text> m_TextComponent;

		int m_Ticks{};
		float m_ElapsedSeconds{};
	};
}

#endif