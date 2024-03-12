#pragma once

#include <memory>
#include <functional>

struct SDL_Window;
struct SDL_Renderer;

namespace fro
{
	class Fronge final
	{
	public:
		Fronge();

		~Fronge() = default;

		void run();

	private:
		Fronge(const Fronge&) = delete;
		Fronge(Fronge&&) noexcept = delete;

		Fronge& operator=(const Fronge&) = delete;
		Fronge& operator=(Fronge&&) noexcept = delete;

		template<typename ResourceType>
		using SDLUniquePointer = std::unique_ptr<ResourceType, std::function<void(ResourceType*)>>;

		const SDLUniquePointer<SDL_Window> m_pWindow;
		const SDLUniquePointer<SDL_Renderer> m_pRenderer;
	};
}