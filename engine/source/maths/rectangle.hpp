#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP

#include "arithmetic.hpp"

namespace fro
{
   template<Arithmetic Component>
   struct Rectangle final
   {
      Component x;
      Component y;
      Component width;
      Component height;
   };
}

#endif