#include <SDL3/SDL_render.h>

#include "renderer.hpp"
#include "resources/surface/surface.hpp"
#include "window.hpp"

namespace fro
{
   Renderer::Renderer(Window const& window)
      : native_renderer_{ SDL_CreateRenderer(window.native_window_.get(), nullptr), SDL_DestroyRenderer }
   {
   }

   Texture const& Renderer::upload_texture(Surface const& surface)
   {
      auto texture{ textures_.find(surface.hash_) };
      if (texture == textures_.end())
         texture = textures_.emplace(surface.hash_, Texture{ *native_renderer_, surface }).first;

      return texture->second;
   }
}