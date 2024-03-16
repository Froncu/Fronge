#include "Fronge.h"

#include "EventManager.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Test.hpp"
#include "ResourceManager.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <VLD/vld.h>
#include <cassert>

#pragma region EntryFunction
int main(int, char**)
{
	int result;

	result = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
	assert(result == 0 && SDL_GetError());

	constexpr int imageFlags{ NULL };
	result = IMG_Init(imageFlags);
	assert(result == imageFlags);

	result = TTF_Init();
	assert(result == 0);

	fro::Fronge().run();

	fro::ResourceManager::clearCaches();

	TTF_Quit();
	IMG_Quit();
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
	SceneManager::addScene("Test").addGameObject().addComponent<Test>();

	while (true)
	{
		if (!EventManager::processEvents()) 
			break;

		SceneManager::update();
		SceneManager::render(m_pRenderer.get());
	}
}
#pragma endregion PublicMethods