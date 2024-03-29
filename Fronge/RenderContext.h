#pragma once

#include "Singleton.hpp"

#include <memory>
#include <functional>

struct SDL_Window;
struct SDL_Renderer;

namespace fro
{
	class RenderContext final : public Singleton<RenderContext>
	{
		fro_GENERATED_SINGLETON_BODY(RenderContext)

	public:
		SDL_Window* getWindow() const;
		SDL_Renderer* getRenderer() const;

	private:
		template<typename ResourceType>
		using SDLUniquePointer = std::unique_ptr<ResourceType, std::function<void(ResourceType*)>>;

		const SDLUniquePointer<SDL_Window> m_pWindow;
		const SDLUniquePointer<SDL_Renderer> m_pRenderer;
	};
}