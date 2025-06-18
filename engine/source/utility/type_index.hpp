#ifndef TYPE_INDEX_HPP
#define TYPE_INDEX_HPP

#include "froch.hpp"

namespace fro
{
   template <typename T>
   std::type_index type_index()
   {
      return std::type_index(typeid(T));
   }
}

#endif