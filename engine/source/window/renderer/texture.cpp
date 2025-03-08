#include <SDL3/SDL_render.h>

#include "renderer.hpp"
#include "resources/surface/surface.hpp"
#include "texture.hpp"

namespace fro
{
   Texture::Texture(Renderer const& renderer, Surface const& surface)
      : native_texture_{
         SDL_CreateTextureFromSurface(renderer.native_renderer_.get(), surface.native_surface_.get()),
         SDL_DestroyTexture
      }
      , hash_{ surface.hash_ }
   {
   }
}