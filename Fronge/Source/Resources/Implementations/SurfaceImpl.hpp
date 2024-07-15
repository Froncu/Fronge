#if not defined SURFACE_IMPL_HPP
#define SURFACE_IMPL_HPP

#include "froch.hpp"

#include "Resources/Surface.hpp"
#include "Utility.hpp"

struct SDL_Surface;

namespace fro
{
	class Surface::Implementation final
	{
	public:
		Implementation(std::string_view const imagePath);
		Implementation(Font const& font, std::string_view const text);

		~Implementation() = default;

		FRO_NODISCARD SDL_Surface* getSDLSurface() const;

	private:
		FRO_NODISCARD static CustomUniquePointer<SDL_Surface> loadSurface(std::string_view const imagePath);
		FRO_NODISCARD static CustomUniquePointer<SDL_Surface> renderTextToSurface(Font const& font, std::string_view const text);

		Implementation(Implementation const&) = delete;
		Implementation(Implementation&&) noexcept = delete;

		Implementation& operator=(Implementation const&) = delete;
		Implementation& operator=(Implementation&&) noexcept = delete;

		CustomUniquePointer<SDL_Surface> mSDLSurface;
	};
}

#endif