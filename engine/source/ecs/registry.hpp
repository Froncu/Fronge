#ifndef REGISTRY_HPP
#define REGISTRY_HPP

#include "component_sparse_set.hpp"
#include "utility/sparse_set.hpp"
#include "utility/unique_pointer.hpp"
#include "utility/type_index.hpp"

namespace fro
{
   class Registry final
   {
      public:
         Registry() = default;
         Registry(Registry const&) = default;
         Registry(Registry&&) = default;

         ~Registry() = default;

         Registry& operator=(Registry const&) = default;
         Registry& operator=(Registry&&) = default;

         template <SparseSetStorable Component, typename... Arguments>
            requires std::constructible_from<Component, Arguments...>
         Component& add_component(std::size_t entity, Arguments&&... arguments)
         {
            return component_sparse_set<Component>().insert(entity, std::forward<Arguments>(arguments)...);
         }

         template <SparseSetStorable Component>
         std::optional<Component> remove_component(std::size_t entity)
         {
            return component_sparse_set<Component>().erase(entity);
         }

         void remove_components(std::size_t const entity)
         {
            for (UniquePointer<BaseComponentSparseSet> const& component_sparse_set : std::views::values(component_sparse_sets_))
               component_sparse_set->remove_component(entity);
         }

      private:
         template <SparseSetStorable Component>
         [[nodiscard]] SparseSet<Component>& component_sparse_set()
         {
            using ComponentSparseSet = SparseSet<Component>;

            UniquePointer<BaseComponentSparseSet>& component_sparse_set{ component_sparse_sets_[type_index<Component>()] };
            if (not component_sparse_set)
               component_sparse_set.reset(new ComponentSparseSet{});

            return static_cast<ComponentSparseSet&>(*component_sparse_set);
         }

         std::unordered_map<std::type_index, UniquePointer<BaseComponentSparseSet>> component_sparse_sets_;
   };
}

#endif