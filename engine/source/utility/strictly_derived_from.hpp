#ifndef STRICTLY_DERIVED_FROM_HPP
#define STRICTLY_DERIVED_FROM_HPP

#include "froch.hpp"

namespace fro
{
   template <typename Type, typename Base>
   concept StrictlyDerivedFrom =
      std::derived_from<Type, Base> and
      not std::same_as<Type, Base>;
}

#endif