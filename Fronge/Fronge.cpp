#include "Fronge.h"

#include "EventManager.h"
#include "SceneManager.h"
#include "GUI.h"
#include "ResourceManager.h"
#include "Timer.h"
#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "Sprite.h"
#include "Rotator.h"
#include "Text.h"
#include "FPSCounter.h"

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

	fro::GUI::initialise(m_pWindow.get(), m_pRenderer.get());
}

fro::Fronge::~Fronge()
{
	fro::GUI::destroy();
}
#pragma endregion Constructors/Destructor



#pragma region PublicMethods
void fro::Fronge::run()
{
	Scene& scene{ SceneManager::addScene("Test") };

	GameObject* pGameObject{ &scene.addGameObject() };
	pGameObject->addComponent<Sprite>()->setFileName("logo.tga");
	pGameObject->addComponent<Rotator>()->setCenter({ 320, 240 });

	float period{ -3.14f };
	for (size_t index{}; index < 2; ++index)
	{
		GameObject& gameObject{ scene.addGameObject() };

		gameObject.addComponent<Sprite>()->setFileName("logo.tga");
		Rotator* const pRotator{ gameObject.addComponent<Rotator>() };
		pRotator->setPeriod(period / (index + 1));
		pRotator->setRadius(100);
		gameObject.getComponent<Transform>()->setLocalPosition({ 50, 50 });
		gameObject.setParent(pGameObject, false);
		pGameObject = &gameObject;

		period *= -1;
	}

	pGameObject = &scene.addGameObject();
	Text& text{ *pGameObject->addComponent<Text>() };
	text.setFont("Lingua.otf", 64);
	pGameObject->addComponent<FPSCounter>();
	pGameObject->getComponent<Transform>()->setLocalPosition({ 320, 30 });

	while (true)
	{
		Timer::update();

		if (!EventManager::processEvents())
			break;

		SceneManager::update();
		SceneManager::render(m_pRenderer.get());
	}
}
#pragma endregion PublicMethods