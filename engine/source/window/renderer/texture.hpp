#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "utility/unique_pointer.hpp"

struct SDL_Texture;

namespace fro
{
   class Renderer;
   class Surface;

   class Texture
   {
      friend Renderer;

      public:
         Texture(Texture const&) = delete;
         Texture(Texture&&) = default;

         ~Texture() = default;

         Texture& operator=(Texture const&) = delete;
         Texture& operator=(Texture&&) = default;

      private:
         Texture(Renderer const& renderer, Surface const& surface);

         UniquePointer<SDL_Texture> native_texture_;
         std::size_t hash_;
   };
}

#endif