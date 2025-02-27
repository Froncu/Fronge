#include "froch.hpp"
#include "transform.hpp"

namespace fro
{
   Transform::~Transform()
   {
      Transform* const newParent{ mParent.valid() ? &*mParent : nullptr };
      for (Reference<Transform> const& child : mChildren)
         child->setParent(newParent, true);

      if (mParent.valid())
      {
         auto const newEnd
         {
            std::remove_if(mParent->mChildren.begin(), mParent->mChildren.end(),
               [this](Reference<Transform> const& child)
               {
                  return this == &*child;
               })
         };

         mParent->mChildren.erase(newEnd, mParent->mChildren.end());
      }

      mParent.reset();
      mChildren.clear();
   }

   Reference<Entity> Transform::getParentingEntity() const
   {
      return mParentingEntity;
   }

   bool Transform::setParent(Transform* const parent, bool keepWorldTransform)
   {
      if ((mParent.valid() and parent == &*mParent) or parent == this or (parent and isParenting(*parent)))
         return false;

      if (mParent.valid())
      {
         auto const newEnd
         {
            std::remove_if(mParent->mChildren.begin(), mParent->mChildren.end(),
               [this](Reference<Transform> const& child)
               {
                  return this == &*child;
               })
         };

         mParent->mChildren.erase(newEnd, mParent->mChildren.end());
      }

      if (keepWorldTransform and mIsWorldTransformDirty)
      {
         calculateWorldTransform();
         mIsWorldTransformDirty = false;
      }

      mParent = parent;

      if (mParent.valid())
         mParent->mChildren.push_back(this);

      if (keepWorldTransform)
         calculateLocalTransform();
      else
         setWorldTransformDirty();

      return true;
   }

   bool Transform::isParenting(Transform const& transform) const
   {
      return std::any_of(mChildren.begin(), mChildren.end(),
         [&transform](Reference<Transform> const& child)
         {
            return &transform == &*child;
         });
   }

   std::vector<Reference<Transform>> const& Transform::getChildren() const
   {
      return mChildren;
   }

   void Transform::setLocalTranslation(Vector2<double> const translation)
   {
      mLocalTransform.setTranslation(translation);
      setWorldTransformDirty();
   }

   void Transform::setLocalRotation(double const rotation)
   {
      mLocalTransform.setRotation(rotation);
      setWorldTransformDirty();
   }

   void Transform::setLocalScale(Vector2<double> const scale)
   {
      mLocalTransform.setScale(scale);
      setWorldTransformDirty();
   }

   void Transform::localTranslate(Vector2<double> const translation)
   {
      if (translation == Vector2<double>{ 0.0, 0.0 })
         return;

      mLocalTransform.translate(translation);
      setWorldTransformDirty();
   }

   void Transform::localRotate(double const rotation)
   {
      if (not rotation)
         return;

      mLocalTransform.rotate(rotation);
      setWorldTransformDirty();
   }

   void Transform::localScale(Vector2<double> const scale)
   {
      if (scale == Vector2<double>{ 1.0, 1.0 })
         return;

      mLocalTransform.scale(scale);
      setWorldTransformDirty();
   }

   void Transform::setWorldTranslation(Vector2<double> const translation)
   {
      auto& worldTransform{ getWorldTransform() };
      if (worldTransform.getTranslation() == translation)
         return;

      worldTransform.setTranslation(translation);
      calculateLocalTransform();

      for (Reference<Transform> const child : mChildren)
         child->setWorldTransformDirty();
   }

   void Transform::setWorldRotation(double const rotation)
   {
      auto& worldTransform{ getWorldTransform() };
      if (worldTransform.getRotation() == rotation)
         return;

      worldTransform.setRotation(rotation);
      calculateLocalTransform();

      for (Reference<Transform> const child : mChildren)
         child->setWorldTransformDirty();
   }

   void Transform::setWorldScale(Vector2<double> const scale)
   {
      auto& worldTransform{ getWorldTransform() };
      if (worldTransform.getScale() == scale)
         return;

      worldTransform.setScale(scale);
      calculateLocalTransform();

      for (Reference<Transform> const child : mChildren)
         child->setWorldTransformDirty();
   }

   void Transform::worldTranslate(Vector2<double> const translation)
   {
      if (translation == Vector2<double>{ 0.0, 0.0 })
         return;

      getWorldTransform().translate(translation);
      calculateLocalTransform();

      for (Reference<Transform> const child : mChildren)
         child->setWorldTransformDirty();
   }

   void Transform::worldRotate(double const rotation)
   {
      if (not rotation)
         return;

      getWorldTransform().rotate(rotation);
      calculateLocalTransform();

      for (Reference<Transform> const child : mChildren)
         child->setWorldTransformDirty();
   }

   void Transform::worldScale(Vector2<double> const scale)
   {
      if (scale == Vector2<double>{ 1.0, 1.0 })
         return;

      getWorldTransform().scale(scale);
      calculateLocalTransform();

      for (Reference<Transform> const child : mChildren)
         child->setWorldTransformDirty();
   }

   TransformMatrix3x3 const& Transform::local() const
   {
      return mLocalTransform;
   }

   TransformMatrix3x3 const& Transform::world() const
   {
      return getWorldTransform();
   }

   void Transform::setWorldTransformDirty()
   {
      if (mIsWorldTransformDirty)
         return;

      mIsWorldTransformDirty = true;

      for (Reference<Transform> const child : mChildren)
         child->setWorldTransformDirty();
   }

   TransformMatrix3x3& Transform::getWorldTransform() const
   {
      if (mIsWorldTransformDirty)
      {
         calculateWorldTransform();
         mIsWorldTransformDirty = false;
      }

      return mWorldTransform;
   }

   void Transform::calculateWorldTransform() const
   {
      if (mParent.valid())
         mWorldTransform = mParent->getWorldTransform() * mLocalTransform;
      else
         mWorldTransform = mLocalTransform;
   }

   void Transform::calculateLocalTransform()
   {
      if (mParent.valid())
         mLocalTransform = mParent->getWorldTransform().getInverse() * mWorldTransform;
      else
         mLocalTransform = mWorldTransform;
   }
}