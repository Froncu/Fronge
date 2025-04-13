#include "froch.hpp"
#include "id.hpp"

namespace fro
{
   ID::ID(Reference<IDGenerator> generator, std::uint32_t const id)
      : generator_{ std::move(generator) }
      , id_{ id }
   {
   }

   ID::ID(ID&& other) noexcept
      : generator_{ std::move(other.generator_) }
      , id_{ other.id_ }
   {
      other.id_ = INVALID_ID;
   }

   ID::~ID()
   {
      if (not generator_.valid())
         return;

      std::uint32_t& highest_taken_id{ generator_->highest_taken_id_ };
      auto& free_ids{ generator_->free_ids_ };

      if (id_ not_eq highest_taken_id)
         free_ids.insert(id_);
      else
      {
         --highest_taken_id;

         auto begin{ free_ids.begin() };
         while (begin not_eq free_ids.end())
         {
            auto const highest_free_id{ begin++ };
            if (highest_taken_id not_eq *highest_free_id)
               break;

            free_ids.erase(highest_free_id);
            --highest_taken_id;
         }
      }
   }

   ID& ID::operator=(ID&& other) noexcept
   {
      if (this == &other)
         return *this;

      generator_ = std::move(other.generator_);
      id_ = other.id_;

      other.id_ = INVALID_ID;

      return *this;
   }

   ID::operator std::uint32_t() const
   {
      return id_;
   }

   bool ID::operator==(ID const& other) const
   {
      if (not generator_.valid() or not other.generator_.valid())
         return false;

      return
         generator_ == other.generator_ and
         id_ == other.id_;
   }

   std::partial_ordering ID::operator<=>(ID const& other) const
   {
      if (not generator_.valid() or not other.generator_.valid())
         return std::partial_ordering::unordered;

      if (generator_ == other.generator_)
         return id_ <=> other.id_;

      return generator_ <=> other.generator_;
   }

   Reference<IDGenerator> ID::generator() const
   {
      return generator_;
   }
}