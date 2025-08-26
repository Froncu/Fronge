#ifndef VARIANT_VISITOR_HPP
#define VARIANT_VISITOR_HPP

#include "froch.hpp"

namespace fro
{
   template <typename... Callables>
   class VariantVisitor final
   {
      struct Visitor final : Callables...
      {
         using Callables::operator()...;
      };

      public:
         explicit VariantVisitor(Callables&&... callables)
            : visitor_{ std::forward<Callables>(callables)... }
         {
         }

         VariantVisitor(VariantVisitor const&) = default;
         VariantVisitor(VariantVisitor&&) = default;

         ~VariantVisitor() = default;

         VariantVisitor& operator=(VariantVisitor const&) = default;
         VariantVisitor& operator=(VariantVisitor&&) = default;

         template <typename... Arguments>
         decltype(auto) operator()(Arguments&&... arguments)
         {
            return std::visit(visitor_, std::forward<Arguments>(arguments)...);
         }

      private:
         Visitor visitor_;
   };
}

#endif