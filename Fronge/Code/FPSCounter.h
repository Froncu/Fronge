#pragma once

#include "Behaviour.h"

namespace fro
{
	class FPSCounter final : public Behaviour
	{
	public:
		FPSCounter(const GameObject& parentingGameObject);

		virtual ~FPSCounter() override = default;

		virtual void update() override;

	private:
		FPSCounter(const FPSCounter&) = delete;
		FPSCounter(FPSCounter&&) noexcept = delete;

		FPSCounter& operator=(const FPSCounter&) = delete;
		FPSCounter& operator=(FPSCounter&&) noexcept = delete;

		int m_Ticks{};
		float m_ElapsedSeconds{};
	};
}