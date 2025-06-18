#ifndef SPRITE_HPP
#define SPRITE_HPP

#include "maths/rectangle.hpp"
#include "reference/reference.hpp"
#include "services/render_context/texture.hpp"

namespace fro
{
   struct Sprite final
   {
      Reference<Texture const> texture{};
      Rectangle<double> source_rectangle{};
   };
}

#endif