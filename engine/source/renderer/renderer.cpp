#include "froch.hpp"

#include "Implementation/RendererImpl.hpp"
#include "Implementation/SDL2_gfxPrimitives.h"
#include "Maths/MathFunctions.hpp"
#include "Renderer.hpp"
#include "Resources/Texture/implementation/TextureImpl.hpp"
#include "Window/Implementation/WindowImpl.hpp"

#include <SDL.h>

namespace fro
{
	IDGenerator Renderer::sIDGenerator{};

	Renderer::Renderer(Window& window,
		Vector2<int> const resolution,
		ScalingMode const scalingMode)
		: mWindow{ window }
		, mResolution{ resolution.x > 0 and resolution.y > 0 ? resolution : mWindow->getSize() }
		, mScalingMode{ scalingMode }
		, mImplementation{ std::make_unique<Implementation>(*mWindow) }
	{
		updateViewPort(mWindow->getSize(), mResolution, mScalingMode);

		Logger::info("created a {}x{} Renderer with ID {} for Window with ID {}!",
			mResolution.x, mResolution.y, mID, window.getID());
	}

	Renderer::~Renderer()
	{
		clearPrimitivesPolyIntsBuffer();

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

		SDL_SetRenderDrawColor(SDLRenderer,
			static_cast<Uint8>(0),
			static_cast<Uint8>(0),
			static_cast<Uint8>(0),
			std::numeric_limits<Uint8>::max());

		SDL_RenderClear(SDLRenderer);
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

	void Renderer::renderCircle(Circle<double> const& circle, bool const fill,
		double const red, double const green, double const blue, double const alpha) const
	{
		if (auto const color{ Implementation::getSDLColor(red, green, blue, alpha) };
			fill)
			filledCircleRGBA(mImplementation->getSDLRenderer(),
				static_cast<Sint16>(circle.center.x),
				static_cast<Sint16>(circle.center.y),
				static_cast<Sint16>(circle.radius),
				color.r,
				color.g,
				color.b,
				color.a);
		else
			circleRGBA(mImplementation->getSDLRenderer(),
				static_cast<Sint16>(circle.center.x),
				static_cast<Sint16>(circle.center.y),
				static_cast<Sint16>(circle.radius),
				color.r,
				color.g,
				color.b,
				color.a);
	}

	void Renderer::renderPolygon(Polygon<double> const& polygon, bool const fill,
		double const red, double const green, double const blue, double const alpha) const
	{
		std::size_t const vertexCount{ polygon.vertices.size() };

		std::vector<Sint16> convertedVerticesX(vertexCount);
		std::vector<Sint16> convertedVerticesY(vertexCount);
		for (std::size_t index{}; index < vertexCount; ++index)
		{
			convertedVerticesX[index] = static_cast<Sint16>(polygon.vertices[index].x);
			convertedVerticesY[index] = static_cast<Sint16>(polygon.vertices[index].y);
		}

		if (auto const color{ Implementation::getSDLColor(red, green, blue, alpha) };
			fill)
			filledPolygonRGBA(mImplementation->getSDLRenderer(),
				convertedVerticesX.data(), convertedVerticesY.data(), static_cast<int>(vertexCount),
				color.r, color.b, color.g, color.a);
		else
			polygonRGBA(mImplementation->getSDLRenderer(),
				convertedVerticesX.data(), convertedVerticesY.data(), static_cast<int>(vertexCount),
				color.r, color.b, color.g, color.a);
	}

	void Renderer::renderChain(Chain<double> const& chain, double const thickness,
		double const red, double const green, double const blue, double const alpha) const
	{
		std::size_t const vertexCount{ chain.vertices.size() };

		std::vector<Vector2<Sint16>> vertices(vertexCount);
		for (std::size_t index{}; index < vertexCount; ++index)
		{
			vertices[index].x = static_cast<Sint16>(chain.vertices[index].x);
			vertices[index].y = static_cast<Sint16>(chain.vertices[index].y);
		}

		auto const color{ Implementation::getSDLColor(red, green, blue, alpha) };

		for (std::size_t index{}; index < vertices.size(); ++index)
		{
			thickLineRGBA(mImplementation->getSDLRenderer(),
				vertices[index].x, vertices[index].y,
				vertices[(index + 1) % vertices.size()].x, vertices[(index + 1) % vertices.size()].y,
				static_cast<Uint8>(thickness),
				color.r,
				color.g,
				color.b,
				color.a);
		}
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