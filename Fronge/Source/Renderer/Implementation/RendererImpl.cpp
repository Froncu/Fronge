#include "froch.hpp"

#include "Maths/MathFunctions.hpp"
#include "RendererImpl.hpp"
#include "Resources/Implementations/TextureImpl.hpp"
#include "Window/Window.hpp"
#include "Window/Implementation/WindowImpl.hpp"

#include <SDL.h>

namespace fro
{
	Renderer::Implementation::Implementation(Window const& window)
		: mSDLRenderer{ SDL_CreateRenderer(window.getImplementation().getSDLWindow(), -1, SDL_RENDERER_ACCELERATED), SDL_DestroyRenderer }
	{
		if (not mSDLRenderer.get())
			FRO_EXCEPTION("failed to create SDL_Renderer ({})", SDL_GetError());
	}

	SDL_Renderer* Renderer::Implementation::getSDLRenderer() const
	{
		return mSDLRenderer.get();
	}

	IDGenerator Renderer::sIDGenerator{};

	Renderer::Renderer(Window& window,
		Vector2<int> const resolution,
		ScalingMode const scalingMode)
		: mOnWindowResizeEvent
		{
			[this](Vector2<int> const& size)
			{
				updateViewPort(size, mResolution, mScalingMode);
				return true;
			}
		}
		, mWindow{ window }
		, mResolution{ resolution.x > 0 and resolution.y > 0 ? resolution : mWindow->getSize() }
		, mScalingMode{ scalingMode }
		, mImplementation{ std::make_unique<Implementation>(*mWindow) }
	{
		updateViewPort(mWindow->getSize(), mResolution, mScalingMode);

		mWindow->mResizeEvent.addListener(mOnWindowResizeEvent);

		Logger::info("created a {}x{} Renderer with ID {} for Window with ID {}!",
			mResolution.x, mResolution.y, mID, window.getID());
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

	void Renderer::clear() const
	{
		SDL_Renderer* const SDLRenderer{ mImplementation->getSDLRenderer() };

		SDL_Color previousColor;
		SDL_GetRenderDrawColor(SDLRenderer, &previousColor.r, &previousColor.g, &previousColor.b, &previousColor.a);

		SDL_SetRenderDrawColor(SDLRenderer,
			static_cast<Uint8>(0),
			static_cast<Uint8>(0),
			static_cast<Uint8>(0),
			std::numeric_limits<Uint8>::max());

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
			vertex.position.x = static_cast<float>(std::floor(position.x));
			vertex.position.y = static_cast<float>(std::floor(position.y));
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

	ID const& Renderer::getID() const
	{
		return mID;
	}

	Vector2<int> Renderer::getResolution() const
	{
		return mResolution;
	}

	Reference<Window> Renderer::getWindow() const
	{
		return mWindow;
	}

	void Renderer::setResolution(Vector2<int> const resolution)
	{
		mResolution = resolution.x > 0 and resolution.y > 0 ? resolution : mWindow->getSize();

		updateViewPort(mWindow->getSize(), mResolution, mScalingMode);

		Logger::info("set Renderer with ID {} resolution to {}x{}!",
			mID, mResolution.x, mResolution.y);
	}

	void Renderer::setScalingMode(ScalingMode const scalingMode)
	{
		mScalingMode = scalingMode;

		updateViewPort(mWindow->getSize(), mResolution, mScalingMode);

		switch (mScalingMode)
		{
		case fro::Renderer::ScalingMode::NONE:
			Logger::info("set Renderer with ID {} to scaling mode none!",
				mID);
			break;

		case fro::Renderer::ScalingMode::ASPECT_RATIO:
			Logger::info("set Renderer with ID {} to scaling mode aspect ratio!",
				mID);
			break;

		case fro::Renderer::ScalingMode::FILL:
			Logger::info("set Renderer with ID {} to scaling mode fill!",
				mID);
			break;
		}
	}

	void Renderer::updateViewPort(Vector2<int> const windowSize,
		Vector2<int> const resolution,
		ScalingMode const scalingMode) const
	{
		if (scalingMode not_eq ScalingMode::ASPECT_RATIO)
		{
			SDL_Rect viewPort;
			viewPort.w = resolution.x;
			viewPort.h = resolution.y;

			switch (scalingMode)
			{
			case ScalingMode::NONE:
				viewPort.x = (windowSize.x - resolution.x) / 2;
				viewPort.y = (windowSize.y - resolution.y) / 2;

				// SDL stops rendering when the viewports' x or y value is below 0, 
				// so we switch to aspect ratio mode when that happens
				if (viewPort.x > 0 and viewPort.y > 0)
					SDL_RenderSetScale(mImplementation->getSDLRenderer(), 1.0f, 1.0f);
				else
				{
					SDL_RenderSetLogicalSize(mImplementation->getSDLRenderer(), resolution.x, resolution.y);
					return;
				}
				break;

			case ScalingMode::FILL:
				viewPort.x = 0;
				viewPort.y = 0;
				SDL_RenderSetScale(mImplementation->getSDLRenderer(),
					static_cast<float>(windowSize.x) / resolution.x,
					static_cast<float>(windowSize.y) / resolution.y);
				break;
			}

			SDL_RenderSetViewport(mImplementation->getSDLRenderer(), &viewPort);
		}
		else
			SDL_RenderSetLogicalSize(mImplementation->getSDLRenderer(), resolution.x, resolution.y);

		Logger::info("updated Renderer with ID {} viewport!",
			mID);
	}
}