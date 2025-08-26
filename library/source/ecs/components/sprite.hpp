#ifndef SPRITE_HPP
#define SPRITE_HPP

#include "reference/reference.hpp"
#include "services/renderer/texture.hpp"

namespace fro
{
   struct Sprite final
   {
      Reference<Texture const> texture{};
      TransformMatrix transform{};

      SourceRectangle source_rectangle{};
   };
}

#endif