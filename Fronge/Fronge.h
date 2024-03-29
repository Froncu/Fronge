#pragma once

#include "Singleton.hpp"

#include <memory>
#include <functional>

struct SDL_Window;
struct SDL_Renderer;

namespace fro
{
	class Fronge final : public Singleton<Fronge>
	{
		fro_GENERATED_SINGLETON_BODY(Fronge)

	public:
		void run();
	};
}