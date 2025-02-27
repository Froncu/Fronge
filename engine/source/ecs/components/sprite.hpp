#ifndef SPRITE_HPP
#define SPRITE_HPP

#include "ecs/components/component.hpp"
#include "maths/structs/rectangle.hpp"
#include "reference/reference.hpp"
#include "resources/texture/texture.hpp"

namespace fro
{
   struct Sprite final : public Component
   {
      public:
         Reference<Texture const> texture{};
         Rectangle<int> sourceRectangle{};
         std::size_t layer{};
   };
}

#endif