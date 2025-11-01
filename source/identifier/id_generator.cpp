#include "fronge/id.hpp"
#include "fronge/id_generator.hpp"
#include "fronge/pch.hpp"

namespace fro
{
   IDGenerator::IDGenerator(IDGenerator&& other) noexcept
      : Referenceable(std::move(other))
      , highest_taken_id_{ std::exchange(other.highest_taken_id_, ID::INVALID_ID) }
      , free_ids_{ std::move(other.free_ids_) }
   {
   }

   IDGenerator& IDGenerator::operator=(IDGenerator&& other) noexcept
   {
      if (this == &other)
         return *this;

      Referenceable::operator=(std::move(other));
      std::exchange(other.highest_taken_id_, ID::INVALID_ID);
      free_ids_ = std::move(other.free_ids_);

      return *this;
   }

   ID IDGenerator::generate()
   {
      return ID{ *this };
   }

   IDGenerator::InternalValue IDGenerator::internal_generate()
   {
      InternalValue id;

      if (free_ids_.empty())
      {
         if (highest_taken_id_ == ID::MAX_ID)
            throw std::runtime_error{ "the ID generator has run out of IDs" };

         id = ++highest_taken_id_;
      }
      else
      {
         auto const lowest_free_id{ std::prev(free_ids_.end()) };
         id = *lowest_free_id;
         free_ids_.erase(lowest_free_id);
      }

      return id;
   }
}