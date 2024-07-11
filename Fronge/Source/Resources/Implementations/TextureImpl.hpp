#if not defined TEXTURE_IMPL_HPP
#define TEXTURE_IMPL_HPP

#include "froch.hpp"

#include "Resources/Texture.hpp"
#include "Utility.hpp"

struct SDL_Texture;

namespace fro
{
	class Texture::Implementation final
	{
	public:
		FRO_API Implementation(Reference<Renderer> const renderer, std::string_view const imagePath);

		~Implementation() = default;

		FRO_API FRO_NODISCARD SDL_Texture* getSDLTexture() const;

	private:
		FRO_NODISCARD static CustomUniquePointer<SDL_Texture> createTexture(
			Reference<Renderer> const renderer, std::string_view const imagePath);

		Implementation(Implementation const&) = delete;
		Implementation(Implementation&&) noexcept = delete;

		Implementation& operator=(Implementation const&) = delete;
		Implementation& operator=(Implementation&&) noexcept = delete;

		CustomUniquePointer<SDL_Texture> mSDLTexture;
	};
}

#endif