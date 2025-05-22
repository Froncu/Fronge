#ifndef SCENE_HPP
#define SCENE_HPP

#include "components.hpp"
#include "reference/referenceable.hpp"
#include "utility/mapped_tuple.hpp"
#include "utility/sparse_set.hpp"
#include "utility/type_index.hpp"

namespace fro
{
   class Entity;
   template <typename, typename>
   class Group;

   class Scene final : public Referenceable
   {
      friend Entity;
      template <typename, typename>
      friend class Group;

      template <Componentable Component>
      class ComponentSparseSet final
      {
         public:
            ComponentSparseSet() = default;
            ComponentSparseSet(ComponentSparseSet const&) = default;
            ComponentSparseSet(ComponentSparseSet&&) = default;

            ~ComponentSparseSet() = default;

            ComponentSparseSet& operator=(ComponentSparseSet const&) = default;
            ComponentSparseSet& operator=(ComponentSparseSet&&) = default;

            bool remove_add_queued()
            {
               if (remove_queue_.empty() and insert_queue_.empty())
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
               Component* const component{ components_.find(entity_id) };
               if (component)
                  return *component;

               return insert_queue_.emplace(entity_id, Component{ std::forward<Arguments>(arguments)... }).first->second;
            }

            void enqueue_remove(ID::InternalValue const entity_id)
            {
               if (not components_.contains(entity_id))
                  return;

               remove_queue_.insert(entity_id);
            }

            bool move(ID::InternalValue const entity_id, std::size_t const data_index)
            {
               return components_.move(entity_id, data_index);
            }

            [[nodiscard]] Component* find(ID::InternalValue const entity_id)
            {
               Component* component{ components_.find(entity_id) };
               if (not component)
                  if (auto const queued_component{ insert_queue_.find(entity_id) };
                     queued_component not_eq insert_queue_.end())
                     component = &queued_component->second;

                  return component;
            }

            [[nodiscard]] Component const* find(ID::InternalValue const entity_id) const
            {
               Component const* component{ components_.find(entity_id) };
               if (not component)
                  if (auto const queued_component{ insert_queue_.find(entity_id) };
                     queued_component not_eq insert_queue_.end())
                     component = &queued_component->second;

               return component;
            }

            [[nodiscard]] bool contains(ID::InternalValue const entity_id) const
            {
               return components_.contains(entity_id);
            }

            [[nodiscard]] std::span<Component> components()
            {
               return components_.dense_data();
            }

         private:
            SparseSet<Component> components_{};
            std::unordered_set<ID::InternalValue> remove_queue_{};
            std::unordered_map<ID::InternalValue, Component> insert_queue_{};
      };

      class BaseGroup
      {
         public:
            BaseGroup() = default;
            BaseGroup(BaseGroup const&) = default;
            BaseGroup(BaseGroup&&) = default;

            virtual ~BaseGroup() = default;

            BaseGroup& operator=(BaseGroup const&) = default;
            BaseGroup& operator=(BaseGroup&&) = default;

            virtual void update() = 0;
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

         template <typename OwnedComponents, typename ObservedComponents>
         Group<OwnedComponents, ObservedComponents>& group()
         {
            using GroupType = Group<OwnedComponents, ObservedComponents>;

            std::type_index const group_type_index{ type_index<GroupType>() };

            auto group{ groups_.find(group_type_index) };
            if (group == groups_.end())
               group = groups_.emplace(group_type_index, new GroupType{ *this }).first;

            return static_cast<GroupType&>(*group->second);
         }

      private:
         MappedTuple<ComponentSparseSet, Components>::Type component_sparse_sets_{};
         // NOTE: the groups would benefit if this was a vector of Entities instead of pointers to them
         // or if the Entity class was just a wrapper around a number with its current methods moved to
         // the Scene class
         std::vector<std::unique_ptr<Entity>> entities_{};
         std::vector<ID::InternalValue> destroy_queue_{};
         std::unordered_map<std::type_index, std::unique_ptr<BaseGroup>> groups_{};
   };
}

#endif