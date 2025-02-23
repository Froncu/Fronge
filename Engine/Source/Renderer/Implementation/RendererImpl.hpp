#ifndef RENDERER_IMPL_HPP
#define RENDERER_IMPL_HPP

#include "Renderer/Renderer.hpp"

#include <SDL_pixels.h>

typedef struct SDL_Renderer SDL_Renderer;

namespace fro
{
	class Renderer::Implementation final
	{
	public:
		FRO_NODISCARD static constexpr SDL_Color getSDLColor(double const red,
			double const green,
			double const blue,
			double const alpha)
		{
			auto constexpr max{ std::numeric_limits<Uint8>::max() };

			return
			{
				static_cast<Uint8>(red * max),
				static_cast<Uint8>(green * max),
				static_cast<Uint8>(blue * max),
				static_cast<Uint8>(alpha * max)
			};
		}

		FRO_API Implementation(Window const& window);

		~Implementation() = default;

		FRO_API FRO_NODISCARD SDL_Renderer* getSDLRenderer() const;

	private:
		Implementation(Implementation const&) = delete;
		Implementation(Implementation&&) noexcept = delete;

		Implementation& operator=(Implementation const&) = delete;
		Implementation& operator=(Implementation&&) noexcept = delete;

		CustomUniquePointer<SDL_Renderer> mSDLRenderer{};
	};
}

#endif