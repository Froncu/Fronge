#if not defined fro_RENDER_CONTEXT_H
#define fro_RENDER_CONTEXT_H

#include "Defines.hpp"
#include "TransformationMatrix2D.h"
#include "Singleton.hpp"
#include "Typenames.hpp"

#include <mat3x3.hpp>
#include <sdl_rect.h>

#include <memory>
#include <functional>
#include <xstring>

typedef union SDL_Event SDL_Event;
struct SDL_Texture;
struct SDL_Window;
struct SDL_Renderer;

namespace fro
{
	class RenderContext final : public Singleton<RenderContext>
	{
		friend class Singleton<RenderContext>;

	public:
		enum class ScalingMode
		{
			none,
			aspectRatio,
			fill
		};

		virtual ~RenderContext() override;

		void processSystemEvent(SDL_Event const& event);
		void clear() const;
		void present() const;
		void renderTexture(SDL_Texture* const pTexture, TransformationMatrix2D const& transform,
			SDL_FRect sourceRectangle = {}) const;

		fro_NODISCARD SDL_Window* getWindow() const;
		fro_NODISCARD SDL_Renderer* getRenderer() const;
		fro_NODISCARD glm::ivec2 const& getViewportSize() const;

		void setFullScreen(bool const enable) const;
		void setWindowResizeable(bool const enable) const;
		void setWindowSize(int const width, int const height) const;
		void setWindowTitle(std::string_view const title) const;
		void setResolution(int const width, int const height);
		void setScalingMode(ScalingMode const scalingMode);

	private:
		RenderContext();
		RenderContext(RenderContext const&) = delete;
		RenderContext(RenderContext&&) noexcept = delete;

		RenderContext& operator=(RenderContext const&) = delete;
		RenderContext& operator=(RenderContext&&) noexcept = delete;

		void updateViewPort() const;

		CustomUniquePointer<SDL_Window> m_pWindow;
		CustomUniquePointer<SDL_Renderer> m_pRenderer;

		ScalingMode m_ScalingMode{ ScalingMode::fill };
		glm::ivec2 m_ViewportSize{ 640, 480 };
	};
}

#endif