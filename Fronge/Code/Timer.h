#pragma once

#include "Singleton.hpp"
#include "Defines.hpp"

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
		Timer(const Timer&) = delete;
		Timer(Timer&&) noexcept = delete;

		Timer& operator=(const Timer&) = delete;
		Timer& operator=(Timer&&) noexcept = delete;

		float m_DeltaSeconds{};
	};
}