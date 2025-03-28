#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "resources/surface/surface.hpp"
#include "utility/unique_pointer.hpp"

struct SDL_Texture;
struct SDL_Renderer;

namespace fro
{
   class RenderContext;
   class Surface;

   class Texture final
   {
      friend RenderContext;

      public:
         Texture(Texture const&) = delete;
         Texture(Texture&&) = default;

         ~Texture() = default;

         Texture& operator=(Texture const&) = delete;
         Texture& operator=(Texture&&) = default;

         [[nodiscard]] std::size_t hash() const;

      private:
         Texture(SDL_Renderer& target_native_renderer, Surface const& source_surface);
         Texture(SDL_Renderer& target_native_renderer, Texture const& source_texture);

         UniquePointer<SDL_Texture> native_texture_;
         std::size_t hash_;
   };
}

template <>
struct std::hash<fro::Texture>
{
   using is_transparent = void;

   [[nodiscard]] std::size_t operator()(fro::Texture const& texture) const noexcept
   {
      return texture.hash();
   }

   [[nodiscard]] std::size_t operator()(fro::Surface const& surface) const noexcept
   {
      return surface.hash();
   }
};

template <>
struct std::equal_to<fro::Texture>
{
   using is_transparent = void;

   [[nodiscard]] bool operator()(fro::Texture const& texture0, fro::Texture const& texture1) const noexcept
   {
      return texture0.hash() == texture1.hash();
   }

   [[nodiscard]] bool operator()(fro::Texture const& texture, fro::Surface const& surface) const noexcept
   {
      return texture.hash() == surface.hash();
   }

   [[nodiscard]] bool operator()(fro::Surface const& surface, fro::Texture const& texture) const noexcept
   {
      return operator()(texture, surface);
   }
};

#endif