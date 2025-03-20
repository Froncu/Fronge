#include <SDL3/SDL_render.h>

#include "render_context.hpp"
#include "resources/surface/surface.hpp"
#include "texture.hpp"
#include "utility/assert.hpp"

namespace fro
{
   Texture::Texture(SDL_Renderer& target_native_renderer, Surface const& source_surface)
      : native_texture_{
         [](SDL_Renderer& target_native_renderer, SDL_Surface& source_surface)
         {
            SDL_Texture* const target_native_texture{
               SDL_CreateTexture(&target_native_renderer, source_surface.format, SDL_TEXTUREACCESS_TARGET,
                  source_surface.w, source_surface.h)
            };
            assert(target_native_texture, "failed to create a target texture ({})",
               SDL_GetError());

            SDL_Texture* old_render_target = SDL_GetRenderTarget(&target_native_renderer);

            bool succeeded{ SDL_SetRenderTarget(&target_native_renderer, target_native_texture) };
            assert(succeeded, "failed to set the target rendering context ({})",
               SDL_GetError());;

            if (UniquePointer<SDL_Texture> const source_native_texture{
               SDL_CreateTextureFromSurface(&target_native_renderer, &source_surface),
               SDL_DestroyTexture
            })
            {
               succeeded = SDL_RenderTexture(&target_native_renderer, source_native_texture.get(), nullptr, nullptr);
               assert(succeeded, "failed to create a rendering texture ({})",
                  SDL_GetError());
            }
            else
               exception("failed to create a texture from a surface ({})",
                  SDL_GetError());

            succeeded = SDL_SetRenderTarget(&target_native_renderer, old_render_target);
            assert(succeeded, "failed to reset the target rendering context ({})",
               SDL_GetError());

            return target_native_texture;
         }(target_native_renderer, *source_surface.native_surface_),
         SDL_DestroyTexture
      }
      , hash_{ source_surface.hash_ }
   {
   }

   Texture::Texture(SDL_Renderer& target_native_renderer, Texture const& source_texture)
      : native_texture_{
         [](SDL_Renderer& target_native_renderer, SDL_Texture& source_native_texture)
         {
            SDL_Renderer* const source_native_renderer{ SDL_GetRendererFromTexture(&source_native_texture) };
            SDL_Texture* const old_render_target{ SDL_GetRenderTarget(source_native_renderer) };

            bool succeeded{ SDL_SetRenderTarget(source_native_renderer, &source_native_texture) };
            assert(succeeded, "failed to set the source rendering context ({})",
               SDL_GetError());

            SDL_Texture* native_texture;

            if (UniquePointer<SDL_Surface> const native_surface{
               SDL_RenderReadPixels(source_native_renderer, nullptr),
               SDL_DestroySurface
            })
               native_texture = SDL_CreateTextureFromSurface(&target_native_renderer, native_surface.get());
            else
               exception("failed to read pixels from a Texture ({})",
                  SDL_GetError());

            if (not native_texture)
               exception("failed to create a Texture from a Surface ({})",
                  SDL_GetError());

            succeeded = SDL_SetRenderTarget(source_native_renderer, old_render_target);
            assert(succeeded, "failed to reset the source rendering context");

            return native_texture;
         }(target_native_renderer, *source_texture.native_texture_),
         SDL_DestroyTexture
      }
      , hash_{ source_texture.hash_ }
   {
   }

   std::size_t Texture::hash() const
   {
      return hash_;
   }
}