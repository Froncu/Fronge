#if not defined RENDERER_IMPL_HPP
#define RENDERER_IMPL_HPP

#include "Renderer/Renderer.hpp"

typedef struct SDL_Renderer SDL_Renderer;

namespace fro
{
	class Renderer::Implementation final
	{
	public:
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