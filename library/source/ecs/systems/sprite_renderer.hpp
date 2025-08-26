#ifndef SPRITE_RENDERER_HPP
#define SPRITE_RENDERER_HPP

#include "core.hpp"
#include "ecs/scene.hpp"
#include "services/renderer/renderer.hpp"

namespace fro
{
   class SpriteRenderer final
   {
      public:
         SpriteRenderer() = default;
         SpriteRenderer(SpriteRenderer const&) = default;
         SpriteRenderer(SpriteRenderer&&) noexcept = default;

         ~SpriteRenderer() = default;

         SpriteRenderer& operator=(SpriteRenderer const&) = default;
         SpriteRenderer& operator=(SpriteRenderer&&) noexcept = default;

         FRO_API void render(Scene const& scene) const;

      private:
         Reference<Renderer> renderer_{ Locator::get<Renderer>() };
   };
}

#endif