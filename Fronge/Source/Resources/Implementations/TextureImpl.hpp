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
		Implementation() = default;
		FRO_API Implementation(Descriptor const& descriptor);

		~Implementation() = default;

		FRO_NODISCARD SDL_Texture* get() const;

	private:
		FRO_NODISCARD static CustomUniquePointer<SDL_Texture> createTexture(Descriptor const& descritptor);

		Implementation(Implementation const&) = delete;
		Implementation(Implementation&&) noexcept = delete;

		Implementation& operator=(Implementation const&) = delete;
		Implementation& operator=(Implementation&&) noexcept = delete;

		CustomUniquePointer<SDL_Texture> mData;
	};
}

#endif