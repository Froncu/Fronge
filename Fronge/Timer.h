#pragma once

#include "Singleton.hpp"

namespace fro
{
	class Timer final : public Singleton<Timer>
	{
		fro_GENERATED_SINGLETON_BODY(Timer)

	public:
		void update();
		float getDeltaSeconds() const;

	private:
		float m_DeltaSeconds{};
	};
}