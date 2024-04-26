#pragma once

#include "Behaviour.h"

namespace fro
{
	class FPSCounter final : public Behaviour
	{
	public:
		FPSCounter(GameObject const& parentingGameObject);

		virtual ~FPSCounter() override = default;

		virtual void update() override;

	private:
		FPSCounter(FPSCounter const&) = delete;
		FPSCounter(FPSCounter&&) noexcept = delete;

		FPSCounter& operator=(FPSCounter const&) = delete;
		FPSCounter& operator=(FPSCounter&&) noexcept = delete;

		int m_Ticks{};
		float m_ElapsedSeconds{};
	};
}