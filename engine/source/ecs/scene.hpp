#ifndef SCENE_HPP
#define SCENE_HPP

#include "reference/referenceable.hpp"
#include "utility/sparse_set.hpp"
#include "utility/type_index.hpp"

namespace fro
{
   class Entity;

   class Scene final : public Referenceable
   {
      friend Entity;
      template <typename, typename>
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

            virtual bool remove_add_queued() = 0;

            FRO_API void enqueue_remove(ID::InternalValue entity_id);

         protected:
            std::unordered_set<ID::InternalValue> remove_queue_{};
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

            virtual bool remove_add_queued() override
            {
               if (remove_queue_.empty() and not insert_queue_.empty())
                  return false;

               for (ID::InternalValue const entity_id : remove_queue_)
                  components_.erase(entity_id);
               remove_queue_.clear();

               for (auto& [entity_id, component] : insert_queue_)
                  components_.insert(entity_id, std::move(component));
               insert_queue_.clear();

               return true;
            }

            template <typename... Arguments>
               requires std::constructible_from<Component, Arguments...>
            Component& enqueue_add(ID::InternalValue const entity_id, Arguments&&... arguments)
            {
               return insert_queue_.emplace(entity_id, Component{ std::forward<Arguments>(arguments)... }).first->second;
            }

            bool move(ID::InternalValue const entity_id, std::size_t const data_index)
            {
               return components_.move(entity_id, data_index);
            }

            [[nodiscard]] Component* find(ID::InternalValue const entity_id)
            {
               return components_.find(entity_id);
            }

            [[nodiscard]] Component const* find(ID::InternalValue const entity_id) const
            {
               return components_.find(entity_id);
            }

            [[nodiscard]] bool contains(ID::InternalValue const entity_id) const
            {
               return components_.contains(entity_id);
            }

            [[nodiscard]] std::span<std::size_t> entity_ids()
            {
               return components_.sparse();
            }

            [[nodiscard]] std::span<Component> components()
            {
               return components_.dense_data();
            }

         private:
            SparseSet<Component> components_{};
            std::unordered_map<ID::InternalValue, Component> insert_queue_{};
      };

      public:
         Scene() = default;
         Scene(Scene const&) = default;
         Scene(Scene&&) = default;

         virtual ~Scene() override = default;

         Scene& operator=(Scene const&) = default;
         Scene& operator=(Scene&&) = default;

         FRO_API [[nodiscard]] Entity& create_entity();
         FRO_API void destroy_entity(Entity const& entity);

         FRO_API void execute_queued();

      private:
         template <SparseSetStorable Component>
         [[nodiscard]] ComponentSparseSet<Component>& component_sparse_set()
         {
            using ComponentSparseSet = ComponentSparseSet<Component>;

            std::unique_ptr<BaseComponentSparseSet>& component_sparse_set{
               component_sparse_sets_[type_index<Component>()]
            };

            if (not component_sparse_set)
               component_sparse_set.reset(new ComponentSparseSet{});

            return static_cast<ComponentSparseSet&>(*component_sparse_set);
         }

         // NOTE: the groups would benefit if this was a vector of Entities instead of pointers to them
         // or if the Entity class was just a wrapper around a number with its current methods moved to
         // the Scene class
         std::unordered_map<std::type_index, std::unique_ptr<BaseComponentSparseSet>> component_sparse_sets_{};
         std::vector<std::unique_ptr<Entity>> entities_{};
         std::vector<Entity const*> destroy_queue_{};
   };
}

#endif