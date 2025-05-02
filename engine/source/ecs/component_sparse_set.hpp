#ifndef COMPONENT_SPARSE_SET_HPP
#define COMPONENT_SPARSE_SET_HPP

#include "utility/sparse_set.hpp"

namespace fro
{
   class BaseComponentSparseSet
   {
      public:
         BaseComponentSparseSet() = default;
         BaseComponentSparseSet(BaseComponentSparseSet const&) = default;
         BaseComponentSparseSet(BaseComponentSparseSet&&) = default;

         virtual ~BaseComponentSparseSet() = default;

         BaseComponentSparseSet& operator=(BaseComponentSparseSet const&) = default;
         BaseComponentSparseSet& operator=(BaseComponentSparseSet&&) = default;

         virtual void remove_component(std::size_t entity) = 0;
   };

   template <SparseSetStorable Component>
   class ComponentSparseSet final : public BaseComponentSparseSet
   {
      public:
         ComponentSparseSet() = default;
         ComponentSparseSet(ComponentSparseSet const&) = default;
         ComponentSparseSet(ComponentSparseSet&&) = default;

         virtual ~ComponentSparseSet() override = default;

         ComponentSparseSet& operator=(ComponentSparseSet const&) = default;
         ComponentSparseSet& operator=(ComponentSparseSet&&) = default;

         virtual void remove_component(std::size_t entity) override
         {
            sparse_set.erase(entity);
         }

         SparseSet<Component> sparse_set{};
   };
}

#endif