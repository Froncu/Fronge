#include "RenderContext.h"

#include <SDL2/SDL_video.h>
#include <SDL2/SDL_render.h>
#include <stdexcept>
#include <format>

#pragma region Constructors/Destructor
fro_GENERATED_SINGLETON_CONSTRUCTOR(RenderContext)
	: m_pWindow{ SDL_CreateWindow("Fronge", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, NULL), SDL_DestroyWindow }
	, m_pRenderer{ SDL_CreateRenderer(m_pWindow.get(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC), SDL_DestroyRenderer }
{
	if (!m_pWindow.get())
		throw std::runtime_error(std::format("[ SDL_CreateWindow() FAILED ] -> {}", SDL_GetError()));

	if (!m_pRenderer.get())
		throw std::runtime_error(std::format("[ SDL_CreateRenderer() FAILED ] -> {}", SDL_GetError()));
}

fro_GENERATED_SINGLETON_DESTRUCTOR(RenderContext)
{
}
#pragma endregion Constructors/Destructor



#pragma region PublicMethods
SDL_Window* fro::RenderContext::getWindow() const
{
	return m_pWindow.get();
}

SDL_Renderer* fro::RenderContext::getRenderer() const
{
	return m_pRenderer.get();
}
#pragma endregion PublicMethods