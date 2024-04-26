#pragma once

#include "Singleton.hpp"
#include "Defines.hpp"

#include <chrono>

namespace fro
{
	class Timer final : public Singleton<Timer>
	{
	public:
		Timer() = default;

		virtual ~Timer() override = default;

		void update();

		fro_NODISCARD float getDeltaSeconds() const;

	private:
		Timer(Timer const&) = delete;
		Timer(Timer&&) noexcept = delete;

		Timer& operator=(Timer const&) = delete;
		Timer& operator=(Timer&&) noexcept = delete;

		float m_DeltaSeconds{};
		std::chrono::steady_clock::time_point m_OldTimePoint{ std::chrono::steady_clock::now() };
	};
}