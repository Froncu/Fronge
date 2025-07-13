#include "sprite_renderer.hpp"
#include "ecs/group.hpp"

namespace fro
{
   void SpriteRenderer::render(Scene const& scene) const
   {
      Vector2 const half_resolution{ render_context_->resolution() * 0.5 };
      for (auto&& [_, sprite, transform] : scene.group<Pack<Sprite>, Pack<Transform>>())
         if (sprite.texture.valid())
         {
            TransformMatrix world_transform{ transform.world() };
            world_transform.translate(half_resolution);
            render_context_->render(*sprite.texture, world_transform, sprite.source_rectangle);
         }
   }
}