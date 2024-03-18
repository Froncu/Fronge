#pragma once

#include "Fronge.h"

namespace fro
{
	class Timer final
	{
		friend void Fronge::run();

	public:
		static float getDeltaSeconds();

	private:
		Timer() = delete;
		Timer(const Timer&) = delete;
		Timer(Timer&&) noexcept = delete;

		~Timer() = delete;

		Timer& operator=(const Timer&) = delete;
		Timer& operator=(Timer&&) noexcept = delete;

		static void update();

		static float m_DELTA_SECONDS;
	};
}