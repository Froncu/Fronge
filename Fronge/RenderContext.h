#pragma once

#include "Singleton.hpp"
#include "Defines.hpp"

#include <GLM/vec2.hpp>
#include <memory>
#include <functional>
#include <set>

struct SDL_Texture;
struct SDL_Window;
struct SDL_Renderer;

namespace fro
{
	class RenderContext final : public Singleton<RenderContext>
	{
		fro_GENERATED_SINGLETON_BODY(RenderContext)

	public:
		void renderTexture(SDL_Texture* const pTexture, const glm::vec2& position) const;

		void clear() const;
		void present() const;

		fro_NODISCARD_GETTER SDL_Window* getWindow() const;
		fro_NODISCARD_GETTER SDL_Renderer* getRenderer() const;

	private:
		template<typename ResourceType>
		using SDLUniquePointer = std::unique_ptr<ResourceType, std::function<void(ResourceType*)>>;

		SDLUniquePointer<SDL_Window> m_pWindow;
		SDLUniquePointer<SDL_Renderer> m_pRenderer;
	};
}