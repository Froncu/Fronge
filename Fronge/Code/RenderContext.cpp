#include "RenderContext.h"

#include "Steam.h"

#include <SDL.h>
#include <stdexcept>
#include <format>

#pragma region Constructors/Destructor
fro::RenderContext::RenderContext()
{
	if (SDL_InitSubSystem(SDL_INIT_VIDEO) != 0)
		throw std::runtime_error(std::format("[ SDL_InitSubSystem() FAILED ] -> {}", SDL_GetError()));

	m_pWindow = { SDL_CreateWindow("Fronge", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, NULL), SDL_DestroyWindow };
	if (!m_pWindow.get())
		throw std::runtime_error(std::format("[ SDL_CreateWindow() FAILED ] -> {}", SDL_GetError()));

	// HACK: Steam must be initialized before the renderer, otherwise the overlay does not work for some reason
	[[maybe_unused]] const Steam& steamInstance{ Steam::getInstance() };
	// END HACK

	m_pRenderer = { SDL_CreateRenderer(m_pWindow.get(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC), SDL_DestroyRenderer };
	if (!m_pRenderer.get())
		throw std::runtime_error(std::format("[ SDL_CreateRenderer() FAILED ] -> {}", SDL_GetError()));
}

fro::RenderContext::~RenderContext()
{
	SDL_QuitSubSystem(SDL_INIT_VIDEO);
}
#pragma endregion Constructors/Destructor



#pragma region PublicMethods
void fro::RenderContext::clear() const
{
	constexpr SDL_Color clearColor{ 32, 32, 32, 255 };

	SDL_SetRenderDrawColor(m_pRenderer.get(), clearColor.r, clearColor.g, clearColor.b, clearColor.a);
	SDL_RenderClear(m_pRenderer.get());
	SDL_SetRenderDrawColor(m_pRenderer.get(), 255, 255, 255, 255);
}

void fro::RenderContext::present() const
{
	SDL_RenderPresent(m_pRenderer.get());
}

void fro::RenderContext::renderTexture(SDL_Texture* const pTexture, const glm::vec2& position) const
{
	int textureWidth;
	int textureHeight;
	SDL_QueryTexture(pTexture, nullptr, nullptr, &textureWidth, &textureHeight);

	const SDL_Rect destinationRectangle
	{
		static_cast<int>(position.x) - textureWidth / 2,
		static_cast<int>(position.y) - textureHeight / 2,
		textureWidth,
		textureHeight
	};

	SDL_RenderCopy(m_pRenderer.get(), pTexture, nullptr, &destinationRectangle);
}

SDL_Window* fro::RenderContext::getWindow() const
{
	return m_pWindow.get();
}

SDL_Renderer* fro::RenderContext::getRenderer() const
{
	return m_pRenderer.get();
}
#pragma endregion PublicMethods