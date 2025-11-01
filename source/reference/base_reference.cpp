#include "fronge/base_reference.hpp"
#include "fronge/referenceable.hpp"

namespace fro
{
   BaseReference::BaseReference(Referenceable const* const referenceable)
      : referenceable_{ const_cast<Referenceable*>(referenceable) }
   {
      if (valid())
         referenceable_->references_.insert(this);
   }

   BaseReference::BaseReference(Referenceable const& referenceable)
      : BaseReference(&referenceable)
   {
   }

   BaseReference::BaseReference(BaseReference const& other)
      : BaseReference(other.referenceable_)
   {
   }

   BaseReference::BaseReference(BaseReference&& other) noexcept
      : BaseReference(other)
   {
      other.reset();
   }

   BaseReference::~BaseReference()
   {
      if (valid())
         referenceable_->references_.erase(this);
   }

   BaseReference& BaseReference::operator=(BaseReference const& other)
   {
      if (*this == other)
         return *this;

      if (valid())
         referenceable_->references_.erase(this);

      referenceable_ = other.referenceable_;

      if (valid())
         referenceable_->references_.insert(this);

      return *this;
   }

   BaseReference& BaseReference::operator=(BaseReference&& other) noexcept
   {
      if (this == &other)
         return *this;

      *this = other;

      other.reset();

      return *this;
   }

   bool BaseReference::operator==(BaseReference const& other) const
   {
      return referenceable_ == other.referenceable_;
   }

   bool BaseReference::operator==(Referenceable const& referenceable) const
   {
      return referenceable_ == &referenceable;
   }

   std::strong_ordering BaseReference::operator<=>(BaseReference const& other) const
   {
      return referenceable_ <=> other.referenceable_;
   }

   std::strong_ordering BaseReference::operator<=>(Referenceable const& referenceable) const
   {
      return referenceable_ <=> &referenceable;
   }

   void BaseReference::reset()
   {
      if (valid())
      {
         referenceable_->references_.erase(this);
         referenceable_ = nullptr;
      }
   }

   bool BaseReference::valid() const
   {
      return referenceable_;
   }
}