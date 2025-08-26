#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "resources/surface/surface.hpp"
#include "utility/unique_pointer.hpp"

struct SDL_Texture;
struct SDL_Renderer;

namespace fro
{
   class Renderer;
   class Surface;

   struct SourceRectangle final
   {
      double x;
      double y;
      double width;
      double height;
   };

   class Texture final : public Referenceable
   {
      friend Renderer;

      public:
         Texture(Texture const&) = delete;
         Texture(Texture&&) = default;

         virtual ~Texture() override = default;

         Texture& operator=(Texture const&) = delete;
         Texture& operator=(Texture&&) = default;

         FRO_API void change_linear_filtering(bool filter_linearly) const;

         FRO_API [[nodiscard]] std::size_t hash() const;
         FRO_API [[nodiscard]] Vector2<double> size() const;

      private:
         [[nodiscard]] static SDL_Texture* create_texture(SDL_Renderer& target_native_renderer,
            SDL_Surface& source_native_surface);

         Texture(SDL_Renderer& target_native_renderer, Surface const& source_surface);
         Texture(SDL_Renderer& target_native_renderer, Texture const& source_texture);

         UniquePointer<SDL_Texture> native_texture_;
         std::size_t hash_;
   };
}

template <>
struct std::hash<fro::Texture>
{
   [[nodiscard]] std::size_t operator()(fro::Texture const& texture) const noexcept
   {
      return texture.hash();
   }
};

template <>
struct std::equal_to<fro::Texture>
{
   [[nodiscard]] bool operator()(fro::Texture const& texture_a, fro::Texture const& texture_b) const noexcept
   {
      return texture_a.hash() == texture_b.hash();
   }
};

#endif