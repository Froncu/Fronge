#ifndef TYPE_INDEX_HPP
#define TYPE_INDEX_HPP

#include <typeindex>

namespace fro
{
   template <typename T>
   std::type_index type_index()
   {
      return std::type_index(typeid(T));
   }
}

#endif