#ifndef GROUP_HPP
#define GROUP_HPP

#include "entity.hpp"
#include "froch.hpp"
#include "reference/reference.hpp"
#include "reference/referenceable.hpp"
#include "scene.hpp"

namespace fro
{
   template <typename... OwnedComponents>
   class Group final : public Referenceable
   {
      using OwnedComponentsTuple = std::tuple<OwnedComponents...>;
      using OwnedComponentsView = std::ranges::take_view<std::ranges::zip_view<std::span<OwnedComponents>...>>;

      public:
         // NOTE: constructing a group can be expensive; preferrably you'd create all your groups
         // before attaching and detaching components

         Group(Group const&) = default;
         Group(Group&&) noexcept = default;

         virtual ~Group() override = default;

         Group& operator=(Group const&) = default;
         Group& operator=(Group&&) noexcept = default;

         explicit Group(Scene& scene)
            : scene_{ scene }
         {
            for (std::unique_ptr<Entity> const& entity : scene_->entities_)
               try_group(static_cast<ID::InternalValue>(entity->id()));
         }

         auto begin()
         {
            return owned_components_.begin();
         }

         auto end()
         {
            return owned_components_.end();
         }

      private:
         void try_group(ID::InternalValue entity_id)
         {
            std::tuple<SparseSet<OwnedComponents>&...> component_sparse_sets{ scene_->sparse_set<OwnedComponents>()... };

            if ((not std::get<SparseSet<OwnedComponents>&>(component_sparse_sets).contains(entity_id) or ...))
               return;

            (std::get<SparseSet<OwnedComponents>&>(component_sparse_sets).move(entity_id, owned_sparse_set_size_), ...);

            ++owned_sparse_set_size_;
            owned_components_ = std::views::zip(std::get<SparseSet<OwnedComponents>&>(component_sparse_sets).dense_data()...) |
               std::views::take(owned_sparse_set_size_);
         }

         Reference<Scene> scene_;
         OwnedComponentsView owned_components_{};
         std::size_t owned_sparse_set_size_{};
   };
}

#endif