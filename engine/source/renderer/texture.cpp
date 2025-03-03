#include <SDL3/SDL_render.h>

#include "resources/surface/surface.hpp"
#include "texture.hpp"

namespace fro
{
   Texture::Texture(SDL_Renderer& renderer, Surface const& surface)
      : native_texture_{ SDL_CreateTextureFromSurface(&renderer, surface.native_surface_.get()), SDL_DestroyTexture }
      , hash_{ surface.hash_ }
   {
   }
}