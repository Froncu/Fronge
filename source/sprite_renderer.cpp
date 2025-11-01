#include "fronge/ecs/group.hpp"
#include "fronge/ecs/sprite_renderer.hpp"

namespace fro
{
   void SpriteRenderer::render(Scene const& scene) const
   {
      for (auto&& [_, sprite, transform] : scene.group<Pack<Sprite>, Pack<Transform>>())
         if (sprite.texture.valid())
            renderer_->render(*sprite.texture, transform.world() * sprite.transform, sprite.source_rectangle);
   }
}