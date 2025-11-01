#ifndef TYPE_INDEX_HPP
#define TYPE_INDEX_HPP

#include "fronge/pch.hpp"

namespace fro
{
   template <typename Type>
   std::type_index type_index()
   {
      return typeid(Type);
   }
}

#endif