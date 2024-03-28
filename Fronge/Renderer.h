#pragma once

#include <memory>
#include <functional>

struct SDL_Window;
struct SDL_Renderer;

namespace fro
{
	class Renderer final
	{
	public:
		Renderer();

		~Renderer() = default;

		SDL_Window* getWindow() const;
		SDL_Renderer* getRenderer() const;

	private:
		Renderer& operator=(const Renderer&) = delete;
		Renderer& operator=(Renderer&&) noexcept = delete;

		Renderer(const Renderer&) = delete;
		Renderer(Renderer&&) noexcept = delete;

		template<typename ResourceType>
		using SDLUniquePointer = std::unique_ptr<ResourceType, std::function<void(ResourceType*)>>;

		const SDLUniquePointer<SDL_Window> m_pWindow;
		const SDLUniquePointer<SDL_Renderer> m_pRenderer;
	};
}