#ifndef SPRITE_HPP
#define SPRITE_HPP

#include "fronge/reference.hpp"
#include "fronge/texture.hpp"

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