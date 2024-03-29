#pragma once

#include <memory>
#include <functional>

struct SDL_Window;
struct SDL_Renderer;

namespace fro
{
	class RenderContext final
	{
	public:
		RenderContext();

		~RenderContext() = default;

		SDL_Window* getWindow() const;
		SDL_Renderer* getRenderer() const;

	private:
		RenderContext& operator=(const RenderContext&) = delete;
		RenderContext& operator=(RenderContext&&) noexcept = delete;

		RenderContext(const RenderContext&) = delete;
		RenderContext(RenderContext&&) noexcept = delete;

		template<typename ResourceType>
		using SDLUniquePointer = std::unique_ptr<ResourceType, std::function<void(ResourceType*)>>;

		const SDLUniquePointer<SDL_Window> m_pWindow;
		const SDLUniquePointer<SDL_Renderer> m_pRenderer;
	};
}