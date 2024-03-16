#pragma once

#include "Renderable.h"
#include "ResourceManager.h"

#include <SDL2/SDL_render.h>
#include <string>

fro_GENERATED_RENDERABLE_BODY(Test)

fro_GENERATED_RENDERABLE_CONSTRUCTOR(Test)
fro_GENERATED_RENDERABLE_DESTRUCTOR(Test)

fro_GENERATED_RENDERABLE_RENDER
(
	Test,

	static int counter{};
	++counter %= 500;

	SDL_RenderCopy(pRenderer, fro::ResourceManager::getImageTexture(pRenderer, "background.tga"), nullptr, nullptr);
	SDL_RenderCopy(pRenderer, fro::ResourceManager::getTextTexture(pRenderer, "Lingua.otf", 128, std::to_string(counter)), nullptr, nullptr);
)