#if not defined TEXTURE_IMPL_HPP
#define TEXTURE_IMPL_HPP

#include "Resources/Texture.hpp"
#include "Utility/CustomUniquePointer.hpp"

struct SDL_Texture;

namespace fro
{
	class Texture::Implementation final
	{
	public:
		Implementation(Renderer& renderer, Surface const& surface);

		~Implementation() = default;

		FRO_NODISCARD SDL_Texture* getSDLTexture() const;

	private:
		Implementation(Implementation const&) = delete;
		Implementation(Implementation&&) noexcept = delete;

		Implementation& operator=(Implementation const&) = delete;
		Implementation& operator=(Implementation&&) noexcept = delete;

		CustomUniquePointer<SDL_Texture> mSDLTexture;
	};
}

#endif