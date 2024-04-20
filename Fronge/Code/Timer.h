#pragma once

#include "Singleton.hpp"
#include "Defines.hpp"

namespace fro
{
	class Timer final : public Singleton<Timer>
	{
		fro_GENERATED_SINGLETON_BODY(Timer)

	public:
		void update();

		fro_NODISCARD_GETTER float getDeltaSeconds() const;

	private:
		float m_DeltaSeconds{};
	};
}