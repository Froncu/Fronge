#include "Fronge.h"

#include <SDL2/SDL.h>
#include <cassert>

#pragma region EntryFunction
int main(int, char**)
{
	[[maybe_unused]] const int result{ SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) };
	assert(result == 0 && SDL_GetError());

	fro::Fronge().run();

	SDL_Quit();

	return 0;
}
#pragma endregion EntryFunction



#pragma region Constructors/Destructor
fro::Fronge::Fronge()
	: m_pWindow{ SDL_CreateWindow("Fronge", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, NULL), SDL_DestroyWindow }
	, m_pRenderer{ SDL_CreateRenderer(m_pWindow.get(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC), SDL_DestroyRenderer }
{
	assert(m_pWindow.get() && SDL_GetError());
	assert(m_pRenderer.get() && SDL_GetError());
}
#pragma endregion Constructors/Destructor



#pragma region PublicMethods
void fro::Fronge::run()
{
	SDL_Event event;
	while (true)
		while (SDL_PollEvent(&event))
			if (event.type == SDL_EventType::SDL_QUIT)
				return;
}
#pragma endregion PublicMethods