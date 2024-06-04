#if not defined fro_FPS_COUNTER_H
#define fro_FPS_COUNTER_H

#include "Behaviour.h"
#include "GameObject.h"
#include "Text.h"

namespace fro
{
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

		Reference<Text> m_Text{ m_ParentingGameObject.get().forceGetComponent<Text>() };

		int m_Ticks{};
		float m_ElapsedSeconds{};
	};
}

#endif