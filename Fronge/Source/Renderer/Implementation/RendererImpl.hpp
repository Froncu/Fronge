#if not defined RENDERER_IMPL_HPP
#define RENDERER_IMPL_HPP

#include "Renderer/Renderer.hpp"

typedef struct SDL_Renderer SDL_Renderer;

namespace fro
{
	class Renderer::Implementation final
	{
	public:
		FRO_API Implementation(Window const& window,
			Vector2<int> const viewPortSize,
			ScalingMode const scalingMode);

		~Implementation() = default;

		FRO_API FRO_NODISCARD SDL_Renderer* getSDLRenderer() const;

		FRO_API void updateViewPort(Window const& window,
			Vector2<int> const viewPortSize,
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