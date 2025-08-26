#include <SDL3/SDL_render.h>

#include "renderer.hpp"
#include "resources/surface/surface.hpp"
#include "SDL3/SDL_hints.h"
#include "texture.hpp"
#include "utility/runtime_assert.hpp"

namespace fro
{
   SDL_Texture* Texture::create_texture(SDL_Renderer& target_native_renderer, SDL_Surface& source_native_surface)
   {
      SDL_Texture* const target_native_texture{
         SDL_CreateTexture(&target_native_renderer, source_native_surface.format, SDL_TEXTUREACCESS_TARGET,
            source_native_surface.w, source_native_surface.h)
      };
      runtime_assert(target_native_texture, "failed to create a target texture ({})",
         SDL_GetError());

      SDL_Texture* const old_render_target{ SDL_GetRenderTarget(&target_native_renderer) };

      bool succeeded{ SDL_SetRenderTarget(&target_native_renderer, target_native_texture) };
      runtime_assert(succeeded, "failed to set the target rendering target ({})",
         SDL_GetError());

      if (UniquePointer<SDL_Texture> const source_native_texture{
         SDL_CreateTextureFromSurface(&target_native_renderer, &source_native_surface),
         SDL_DestroyTexture
      })
      {
         succeeded = SDL_RenderTexture(&target_native_renderer, source_native_texture.get(), nullptr, nullptr);
         runtime_assert(succeeded, "failed to create a rendering texture ({})",
            SDL_GetError());
      }
      else
         runtime_assert("failed to create a texture from a surface ({})",
            SDL_GetError());

      succeeded = SDL_SetRenderTarget(&target_native_renderer, old_render_target);
      runtime_assert(succeeded, "failed to reset the target rendering target ({})",
         SDL_GetError());

      return target_native_texture;
   }

   Texture::Texture(SDL_Renderer& target_native_renderer, Surface const& source_surface)
      : native_texture_{
         [&target_native_renderer, &source_surface]
         {
            return create_texture(target_native_renderer, *source_surface.native_surface_);
         }(),
         SDL_DestroyTexture
      }
      , hash_{ source_surface.hash_ }
   {
   }

   Texture::Texture(SDL_Renderer& target_native_renderer, Texture const& source_texture)
      : native_texture_{
         [&target_native_renderer, & source_texture]
         {
            SDL_Texture& source_native_texture{ *source_texture.native_texture_ };
            SDL_Renderer* const source_native_renderer{ SDL_GetRendererFromTexture(&source_native_texture) };
            SDL_Texture* const old_render_target{ SDL_GetRenderTarget(source_native_renderer) };

            bool succeeded{ SDL_SetRenderTarget(source_native_renderer, &source_native_texture) };
            runtime_assert(succeeded, "failed to set the source rendering target ({})",
               SDL_GetError());

            UniquePointer<SDL_Surface> const native_surface{
               SDL_RenderReadPixels(source_native_renderer, nullptr),
               SDL_DestroySurface
            };
            if (not native_surface)
               runtime_assert("failed to read pixels from a texture ({})",
                  SDL_GetError());

            succeeded = SDL_SetRenderTarget(source_native_renderer, old_render_target);
            runtime_assert(succeeded, "failed to reset the source rendering target");

            return create_texture(target_native_renderer, *native_surface);
         }(),
         SDL_DestroyTexture
      }
      , hash_{ source_texture.hash_ }
   {
   }

   void Texture::change_linear_filtering(bool const filter_linearly) const
   {
      bool const succeeded{
         SDL_SetTextureScaleMode(native_texture_.get(), filter_linearly ? SDL_SCALEMODE_LINEAR : SDL_SCALEMODE_NEAREST)
      };
      runtime_assert(succeeded, "failed to change the filtering mode of a Texture ({})",
         SDL_GetError());
   }

   std::size_t Texture::hash() const
   {
      return hash_;
   }

   Vector2<double> Texture::size() const
   {
      Vector2<float> size;
      bool const succeeded{ SDL_GetTextureSize(native_texture_.get(), &size.x, &size.y) };
      runtime_assert(succeeded, "failed to get the size of the texture ({})",
         SDL_GetError());

      return { static_cast<double>(size.x), static_cast<double>(size.y) };
   }
}