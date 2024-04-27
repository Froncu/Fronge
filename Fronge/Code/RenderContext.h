#if not defined fro_RENDER_CONTEXT_H
#define fro_RENDER_CONTEXT_H

#include "Singleton.hpp"
#include "Defines.hpp"
#include "Typenames.hpp"

#include <vec2.hpp>
#include <memory>
#include <functional>

struct SDL_Texture;
struct SDL_Window;
struct SDL_Renderer;

namespace fro
{
	class RenderContext final : public Singleton<RenderContext>
	{
	public:
		RenderContext();

		virtual ~RenderContext() override;

		void clear() const;
		void present() const;
		void renderTexture(SDL_Texture* const pTexture, glm::vec2 const& position) const;

		fro_NODISCARD SDL_Window* getWindow() const;
		fro_NODISCARD SDL_Renderer* getRenderer() const;

	private:
		RenderContext(RenderContext const&) = delete;
		RenderContext(RenderContext&&) noexcept = delete;

		RenderContext& operator=(RenderContext const&) = delete;
		RenderContext& operator=(RenderContext&&) noexcept = delete;

		SDLUniquePointer<SDL_Window> m_pWindow;
		SDLUniquePointer<SDL_Renderer> m_pRenderer;
	};
}

#endif