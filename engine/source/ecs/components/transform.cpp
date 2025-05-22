#include "froch.hpp"
#include "transform.hpp"

namespace fro
{
   Transform::~Transform()
   {
      Transform* const new_parent{ parent_.valid() ? &*parent_ : nullptr };
      for (Reference<Transform> const& child : children_)
         child->change_parent(new_parent, true);

      if (parent_.valid())
         std::erase_if(parent_->children_,
            [this](Reference<Transform> const& child)
            {
               return this == &*child;
            });
   }

   bool Transform::change_parent(Transform* const parent, bool const keep_world_transform)
   {
      if ((parent_.valid() and &*parent_ == parent) or parent == this or (parent and is_parenting(*parent)))
         return false;

      if (parent_.valid())
         std::erase_if(parent_->children_,
            [this](Reference<Transform> const& child)
            {
               return this == &*child;
            });

      if (keep_world_transform and is_world_transform_dirty_)
      {
         calculate_world_transform();
         is_world_transform_dirty_ = false;
      }

      parent_ = Reference{ parent };

      if (parent_.valid())
         parent_->children_.push_back(Reference{ this });

      if (keep_world_transform)
         calculate_local_transform();
      else
         mark_world_transform_dirty();

      return true;
   }

   void Transform::change_local_translation(Vector2<double> const translation)
   {
      local_transform_.change_translation(translation);
      mark_world_transform_dirty();
   }

   void Transform::change_local_rotation(double const rotation)
   {
      local_transform_.change_rotation(rotation);
      mark_world_transform_dirty();
   }

   void Transform::change_local_scale(Vector2<double> const scale)
   {
      local_transform_.change_scale(scale);
      mark_world_transform_dirty();
   }

   void Transform::local_translate(Vector2<double> const translation)
   {
      if (translation == Vector2{ 0.0, 0.0 })
         return;

      local_transform_.translate(translation);
      mark_world_transform_dirty();
   }

   void Transform::local_rotate(double const rotation)
   {
      if (not rotation)
         return;

      local_transform_.rotate(rotation);
      mark_world_transform_dirty();
   }

   void Transform::local_scale(Vector2<double> const scale)
   {
      if (scale == Vector2{ 1.0, 1.0 })
         return;

      local_transform_.scale(scale);
      mark_world_transform_dirty();
   }

   void Transform::change_world_translation(Vector2<double> const translation)
   {
      TransformMatrix& current_world_transform{ world_transform() };
      if (current_world_transform.translation() == translation)
         return;

      current_world_transform.change_translation(translation);
      calculate_local_transform();

      for (Reference<Transform> const& child : children_)
         child->mark_world_transform_dirty();
   }

   void Transform::change_world_rotation(double const rotation)
   {
      TransformMatrix& current_world_transform{ world_transform() };
      if (current_world_transform.rotation() == rotation)
         return;

      current_world_transform.change_rotation(rotation);
      calculate_local_transform();

      for (Reference<Transform> const& child : children_)
         child->mark_world_transform_dirty();
   }

   void Transform::change_world_scale(Vector2<double> const scale)
   {
      TransformMatrix& current_world_transform{ world_transform() };
      if (current_world_transform.scale() == scale)
         return;

      current_world_transform.change_scale(scale);
      calculate_local_transform();

      for (Reference<Transform> const& child : children_)
         child->mark_world_transform_dirty();
   }

   void Transform::world_translate(Vector2<double> const translation)
   {
      if (translation == Vector2{ 0.0, 0.0 })
         return;

      world_transform().translate(translation);
      calculate_local_transform();

      for (Reference<Transform> const& child : children_)
         child->mark_world_transform_dirty();
   }

   void Transform::world_rotate(double const rotation)
   {
      if (not rotation)
         return;

      world_transform().rotate(rotation);
      calculate_local_transform();

      for (Reference<Transform> const& child : children_)
         child->mark_world_transform_dirty();
   }

   void Transform::world_scale(Vector2<double> const scale)
   {
      if (scale == Vector2{ 1.0, 1.0 })
         return;

      world_transform().scale(scale);
      calculate_local_transform();

      for (Reference<Transform> const& child : children_)
         child->mark_world_transform_dirty();
   }

   TransformMatrix const& Transform::local() const
   {
      return local_transform_;
   }

   TransformMatrix const& Transform::world() const
   {
      return world_transform();
   }

   Reference<Transform> Transform::parent() const
   {
      return parent_;
   }

   std::span<Reference<Transform> const> Transform::children() const
   {
      return children_;
   }

   bool Transform::is_parenting(Transform const& transform) const
   {
      return std::ranges::any_of(children_,
         [&transform](Reference<Transform> const& child)
         {
            return &transform == &*child;
         });
   }

   void Transform::mark_world_transform_dirty() const
   {
      if (is_world_transform_dirty_)
         return;

      is_world_transform_dirty_ = true;

      for (Reference<Transform> const& child : children_)
         child->mark_world_transform_dirty();
   }

   void Transform::calculate_local_transform()
   {
      if (parent_.valid())
         local_transform_ = parent_->world_transform().inverse() * world_transform_;
      else
         local_transform_ = world_transform_;
   }

   void Transform::calculate_world_transform() const
   {
      if (parent_.valid())
         world_transform_ = parent_->world_transform() * local_transform_;
      else
         world_transform_ = local_transform_;
   }

   TransformMatrix& Transform::world_transform() const
   {
      if (is_world_transform_dirty_)
      {
         calculate_world_transform();
         is_world_transform_dirty_ = false;
      }

      return world_transform_;
   }
}