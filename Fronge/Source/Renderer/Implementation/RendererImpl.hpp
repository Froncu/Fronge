#if not defined RENDERER_IMPL_HPP
#define RENDERER_IMPL_HPP

#include "Renderer/Renderer.hpp"

typedef struct SDL_Renderer SDL_Renderer;

namespace fro
{
	class Renderer::Implementation final
	{
	public:
		Implementation(Window const& window,
			Vector2<int> const resolution,
			ScalingMode const scalingMode);

		~Implementation() = default;

		FRO_NODISCARD SDL_Renderer* getSDLRenderer() const;

		void updateViewPort(Vector2<int> const windowSize,
			Vector2<int> const resolution,
			ScalingMode const scalingMode) const;

	private:
		Implementation(Implementation const&) = delete;
		Implementation(Implementation&&) noexcept = delete;

		Implementation& operator=(Implementation const&) = delete;
		Implementation& operator=(Implementation&&) noexcept = delete;

		CustomUniquePointer<SDL_Renderer> mSDLRenderer{};
	};
}

#endif