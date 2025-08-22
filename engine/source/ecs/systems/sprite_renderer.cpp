#include "sprite_renderer.hpp"
#include "ecs/group.hpp"

namespace fro
{
   void SpriteRenderer::render(Scene const& scene) const
   {
      for (auto&& [_, sprite, transform] : scene.group<Pack<Sprite>, Pack<Transform>>())
         if (sprite.texture.valid())
            render_context_->render(*sprite.texture, transform.world() * sprite.transform, sprite.source_rectangle);
   }
}