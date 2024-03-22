#pragma once

#include "Behaviour.h"

namespace fro
{
	class FPSCounter final : public Behaviour
	{
		fro_GENERATED_BEHAVIOUR_BODY(FPSCounter)

	private:
		int m_Ticks{};
		float m_ElapsedSeconds{};
	};
}