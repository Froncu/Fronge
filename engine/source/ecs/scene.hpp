#ifndef SCENE_HPP
#define SCENE_HPP

#include "reference/referenceable.hpp"
#include "utility/type_index.hpp"
#include "utility/unique_pointer.hpp"
#include "utility/sparse_set.hpp"

namespace fro
{
   class Scene final : public Referenceable
   {
      friend class Entity;
      template <typename...>
      friend class Group;

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

            SparseSet<Component> sparse_set{};

         private:
            virtual void remove_component(std::size_t entity) override
            {
               sparse_set.erase(entity);
            }
      };

      public:
         Scene() = default;
         Scene(Scene const&) = default;
         Scene(Scene&&) = default;

         virtual ~Scene() override = default;

         Scene& operator=(Scene const&) = default;
         Scene& operator=(Scene&&) = default;

         FRO_API [[nodiscard]] Entity& create_entity();

      private:
         template <SparseSetStorable Component>
         [[nodiscard]] SparseSet<Component>& sparse_set()
         {
            using ComponentSparseSet = ComponentSparseSet<Component>;

            std::unique_ptr<BaseComponentSparseSet>& component_sparse_set{
               component_sparse_sets_[type_index<Component>()]
            };

            if (not component_sparse_set)
               component_sparse_set.reset(new ComponentSparseSet{});

            return static_cast<ComponentSparseSet&>(*component_sparse_set).sparse_set;
         }

         std::unordered_map<std::type_index, std::unique_ptr<BaseComponentSparseSet>> component_sparse_sets_{};
         std::vector<std::unique_ptr<Entity>> entities_{};
   };
}

#endif