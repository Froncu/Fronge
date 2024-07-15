#include "froch.hpp"

#include "Maths/MathFunctions.hpp"
#include "RendererImpl.hpp"
#include "Resources/Implementations/TextureImpl.hpp"
#include "Window/Window.hpp"
#include "Window/Implementation/WindowImpl.hpp"

#include <SDL.h>

namespace fro
{
	Renderer::Implementation::Implementation(Window const& window,
		Vector2<int> const viewPortSize,
		ScalingMode const scalingMode)
		: mSDLRenderer{ SDL_CreateRenderer(window.getImplementation().getSDLWindow(), -1, SDL_RENDERER_ACCELERATED), SDL_DestroyRenderer }
	{
		if (not mSDLRenderer.get())
			FRO_EXCEPTION("failed to create SDL_Renderer ({})", SDL_GetError());

		updateViewPort(window.getSize(), viewPortSize, scalingMode);
	}

	SDL_Renderer* Renderer::Implementation::getSDLRenderer() const
	{
		return mSDLRenderer.get();
	}

	void Renderer::Implementation::updateViewPort(Vector2<int> const windowSize,
		Vector2<int> const viewPortSize,
		ScalingMode const scalingMode) const
	{
		if (scalingMode not_eq ScalingMode::aspectRatio)
		{
			SDL_Rect viewPort;
			viewPort.w = viewPortSize.x;
			viewPort.h = viewPortSize.y;

			switch (scalingMode)
			{
			case ScalingMode::none:
				viewPort.x = windowSize.x / 2 - viewPortSize.x / 2;
				viewPort.y = windowSize.y / 2 - viewPortSize.y / 2;
				SDL_RenderSetScale(mSDLRenderer.get(), 1.0f, 1.0f);
				break;

			case ScalingMode::fill:
				viewPort.x = 0;
				viewPort.y = 0;
				SDL_RenderSetScale(mSDLRenderer.get(),
					static_cast<float>(windowSize.x) / viewPortSize.x,
					static_cast<float>(windowSize.y) / viewPortSize.y);
				break;
			}

			SDL_RenderSetViewport(mSDLRenderer.get(), &viewPort);
		}
		else
			SDL_RenderSetLogicalSize(mSDLRenderer.get(), viewPortSize.x, viewPortSize.y);
	}

	IDGenerator Renderer::sIDGenerator{};

	Renderer::Renderer(Window& window,
		Vector2<int> const viewPortSize,
		ScalingMode const scalingMode)
		: mOnWindowResizeEvent
		{
			[this](Vector2<int> const size)
			{
				mImplementation->updateViewPort(size, mViewportSize, mScalingMode);
				return false;
			}
		}
		, mWindow{ window }
		, mViewportSize{ viewPortSize.x and viewPortSize.y ? viewPortSize : mWindow->getSize() }
		, mScalingMode{ scalingMode }
		, mImplementation{ std::make_unique<Implementation>(window, mViewportSize, mScalingMode) }
	{
		Logger::info("created a {}x{} Renderer with ID {} for Window with ID {}!",
			mViewportSize.x, mViewportSize.y, mID, window.getID());
	}

	Renderer::~Renderer()
	{
		Logger::info("destroyed Renderer with ID {}!",
			mID);
	}

	Renderer::Implementation& Renderer::getImplementation() const
	{
		return *mImplementation;
	}

	Reference<Texture> Renderer::upload(Surface const& surface)
	{
		return mTextures.emplace_back(*this, surface);
	}

	void Renderer::clear(float red, float green, float blue) const
	{
		SDL_Renderer* const SDLRenderer{ mImplementation->getSDLRenderer() };

		SDL_Color previousColor;
		SDL_GetRenderDrawColor(SDLRenderer, &previousColor.r, &previousColor.g, &previousColor.b, &previousColor.a);

		red = std::clamp(red, 0.0f, 1.0f);
		green = std::clamp(green, 0.0f, 1.0f);
		blue = std::clamp(blue, 0.0f, 1.0f);

		Uint8 constexpr maxColorValue{ 255 };
		SDL_SetRenderDrawColor(SDLRenderer,
			static_cast<Uint8>(red * maxColorValue),
			static_cast<Uint8>(green * maxColorValue),
			static_cast<Uint8>(blue * maxColorValue),
			maxColorValue);

		SDL_RenderClear(SDLRenderer);

		SDL_SetRenderDrawColor(SDLRenderer, previousColor.r, previousColor.g, previousColor.b, previousColor.a);
	}

	void Renderer::present() const
	{
		SDL_RenderPresent(mImplementation->getSDLRenderer());
	}

	void Renderer::renderTexture(Texture const& texture,
		Matrix3x3<double> const& transform,
		Rectangle<int> sourceRectangle) const
	{
		Vector2<int> const textureSize{ texture.getSize() };

		if (not sourceRectangle.width or not sourceRectangle.height)
		{
			sourceRectangle.x = 0;
			sourceRectangle.y = 0;
			sourceRectangle.width = textureSize.x;
			sourceRectangle.height = textureSize.y;
		}

		float const halfSourceWidth{ sourceRectangle.width / 2.0f };
		float const halfSourceHeight{ sourceRectangle.height / 2.0f };

		SDL_Color const vertexColor
		{
			.r{ 255u },
			.g{ 255u },
			.b{ 255u },
			.a{ 255u }
		};

		Vector2<float> const topLeftTexture
		{
			static_cast<float>(sourceRectangle.x) / textureSize.x,
			static_cast<float>(sourceRectangle.y) / textureSize.y
		};

		Vector2<float> const bottomRightTexture
		{
			static_cast<float>(sourceRectangle.x + sourceRectangle.width) / textureSize.x,
			static_cast<float>(sourceRectangle.y + sourceRectangle.height) / textureSize.y,
		};

		std::array<SDL_Vertex, 4> vVertices
		{
			SDL_Vertex({ -halfSourceWidth, -halfSourceHeight },
			vertexColor, { topLeftTexture.x, topLeftTexture.y }),

			SDL_Vertex({ halfSourceWidth, -halfSourceHeight },
			vertexColor, { bottomRightTexture.x, topLeftTexture.y }),

			SDL_Vertex({ halfSourceWidth, halfSourceHeight },
			vertexColor, { bottomRightTexture.x, bottomRightTexture.y }),

			SDL_Vertex({ -halfSourceWidth, halfSourceHeight },
			vertexColor, { topLeftTexture.x, bottomRightTexture.y }),
		};

		for (SDL_Vertex& vertex : vVertices)
		{
			Vector3<double> position{ vertex.position.x, vertex.position.y, 1.0 };

			position = transform * position;
			vertex.position.x = static_cast<float>(position.x);
			vertex.position.y = static_cast<float>(position.y);
		}

		std::array<int, 6> constexpr vIndices
		{
			0, 1, 2,
			2, 3, 0
		};

		if (SDL_RenderGeometry(mImplementation->getSDLRenderer(), texture.getImplementation().getSDLTexture(),
			vVertices.data(), static_cast<int>(vVertices.size()),
			vIndices.data(), static_cast<int>(vIndices.size())) == -1)
			Logger::warn("failed to render Texture with ID {} with Renderer with ID {} ({})",
				texture.getID(), getID(), SDL_GetError());
	}

	std::size_t Renderer::getID() const
	{
		return mID;
	}

	Vector2<int> Renderer::getViewportSize() const
	{
		return mViewportSize;
	}

	Reference<Window> Renderer::getWindow() const
	{
		return mWindow;
	}

	void Renderer::setResolution(Vector2<int> const resolution)
	{
		mViewportSize = resolution.x and resolution.y ? resolution : mWindow->getSize();

		mImplementation->updateViewPort(mWindow->getSize(), mViewportSize, mScalingMode);
	}

	void Renderer::setScalingMode(ScalingMode const scalingMode)
	{
		mScalingMode = scalingMode;

		mImplementation->updateViewPort(mWindow->getSize(), mViewportSize, mScalingMode);
	}
}