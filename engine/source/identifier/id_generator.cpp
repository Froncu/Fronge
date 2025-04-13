#include "froch.hpp"
#include "id.hpp"
#include "id_generator.hpp"

namespace fro
{
   IDGenerator::IDGenerator()
      : highest_taken_id_{ ID::INVALID_ID }
   {
   }

   IDGenerator::IDGenerator(IDGenerator&& other) noexcept
      : Referenceable(std::move(other))
      , highest_taken_id_{ other.highest_taken_id_ }
      , free_ids_{ std::move(other.free_ids_) }
   {
      other.highest_taken_id_ = ID::INVALID_ID;
   }

   IDGenerator& IDGenerator::operator=(IDGenerator&& other) noexcept
   {
      if (this == &other)
         return *this;

      Referenceable::operator=(std::move(other));
      highest_taken_id_ = other.highest_taken_id_;
      free_ids_ = std::move(other.free_ids_);

      other.highest_taken_id_ = ID::INVALID_ID;

      return *this;
   }

   ID IDGenerator::generate()
   {
      std::uint32_t id;

      if (free_ids_.empty())
         id = ++highest_taken_id_;
      else
      {
         auto const lowest_free_id{ --free_ids_.end() };
         id = *lowest_free_id;
         free_ids_.erase(lowest_free_id);
      }

      return { Reference{ this }, id };
   }

   std::uint32_t IDGenerator::highest_taken_id() const
   {
      return highest_taken_id_;
   }
}