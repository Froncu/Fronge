#ifndef HASH_HPP
#define HASH_HPP

#include "fronge/pch.hpp"

namespace fro
{
   template <typename Value>
   std::size_t hash(Value const& value)
   {
      return std::hash<Value>{}(value);
   }
}

#endif