#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "identifier/id.hpp"
#include "scene.hpp"

namespace fro
{
   class Entity final : public Referenceable
   {
      friend Scene;

      public:
         Entity(Entity const&) = delete;
         Entity(Entity&&) = delete;

         FRO_API virtual ~Entity() override;

         Entity& operator=(Entity const&) = delete;
         Entity& operator=(Entity&&) = delete;

         template <SparseSetStorable Component, typename... Arguments>
            requires std::constructible_from<Component, Arguments...>
         Component& add_component(Arguments&&... arguments)
         {
            return scene_->sparse_set<Component>().insert(static_cast<ID::InternalValue>(id_), std::forward<Arguments>(arguments)...);
         }

         template <SparseSetStorable Component>
         Component* find_component() const
         {
            return scene_->sparse_set<Component>().find(static_cast<ID::InternalValue>(id_));
         }

         template <SparseSetStorable Component>
         void remove_component()
         {
            scene_->sparse_set<Component>().erase(static_cast<ID::InternalValue>(id_));
         }

         ID const& id() const
         {
            return id_;
         }

      private:
         static IDGenerator id_generator_;

         explicit Entity(Scene& scene);

         ID id_{ id_generator_.generate() };
         Reference<Scene> scene_;
   };
}

#endif