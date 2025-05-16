// Written based on Michele "skypjack" Caini's
// blog posts about ECS (https://skypjack.github.io/)
// as well as his EnTT library (https://github.com/skypjack/entt)

#ifndef SPARSE_SET_HPP
#define SPARSE_SET_HPP

#include "froch.hpp"

namespace fro
{
   template <typename Type>
   concept SparseSetStorable =
      std::swappable<Type> and
      std::movable<Type>;

   template <SparseSetStorable Data>
   class SparseSet final
   {
      public:
         using Key = std::size_t;
         using DataIndex = std::size_t;

         static DataIndex constexpr UNUSED_DATA_INDEX{ std::numeric_limits<DataIndex>::max() };

         SparseSet() = default;
         SparseSet(SparseSet const&) = default;
         SparseSet(SparseSet&&) noexcept = default;

         ~SparseSet() = default;

         SparseSet& operator=(SparseSet const&) = default;
         SparseSet& operator=(SparseSet&&) noexcept = default;

         template <typename... Arguments>
            requires std::constructible_from<Data, Arguments...>
         Data& insert(Key const key, Arguments&&... arguments)
         {
            if (not in_sparse_range(key))
               sparse_.resize(key + 1, UNUSED_DATA_INDEX);
            else if (naive_contains(key))
               return naive_find(key);

            return naive_insert(key, std::forward<Arguments>(arguments)...);
         }

         std::optional<Data> erase(Key const key)
         {
            if (not contains(key))
               return std::nullopt;

            naive_move(key, dense_.size() - 1);
            sparse_[key] = UNUSED_DATA_INDEX;
            dense_.pop_back();

            std::optional data{ std::move(dense_data_.back()) };
            dense_data_.pop_back();

            return data;
         }

         [[nodiscard]] Data* find(Key const key)
         {
            if (not contains(key))
               return nullptr;

            return &naive_find(key);
         }

         void clear()
         {
            sparse_.clear();
            dense_.clear();
            dense_data_.clear();
         }

         void shrink_sparse()
         {
            auto const last_used_key
            {
               std::find_if(sparse_.rbegin(), sparse_.rend(),
                  [](Key const key)
                  {
                     return key not_eq UNUSED_DATA_INDEX;
                  }).base()
            };

            sparse_.erase(last_used_key, sparse_.end());
            sparse_.shrink_to_fit();
         }

         void shrink_dense()
         {
            dense_.shrink_to_fit();
            dense_data_.shrink_to_fit();
         }

         void shrink()
         {
            shrink_sparse();
            shrink_dense();
         }

         bool move(Key const key, DataIndex const where)
         {
            if (not in_dense_range(where) or not contains(key))
               return false;

            naive_move(key, where);
            return true;
         }

         [[nodiscard]] bool contains(Key const key) const
         {
            return in_sparse_range(key) and naive_contains(key);
         }

         [[nodiscard]] std::size_t sparse_capacity() const
         {
            return sparse_.capacity();
         }

         [[nodiscard]] std::size_t dense_capacity() const
         {
            return dense_.capacity();
         }

         [[nodiscard]] std::size_t dense_data_capacity() const
         {
            return dense_data_.capacity();
         }

         [[nodiscard]] std::span<DataIndex> sparse()
         {
            return sparse_;
         }

         [[nodiscard]] std::span<DataIndex const> sparse() const
         {
            return sparse_;
         }

         [[nodiscard]] std::span<Key const> dense() const
         {
            return dense_;
         }

         [[nodiscard]] std::span<Key> dense()
         {
            return dense_;
         }

         [[nodiscard]] std::span<Data> dense_data()
         {
            return dense_data_;
         }

         [[nodiscard]] std::span<Data const> dense_data() const
         {
            return dense_data_;
         }

      private:
         [[nodiscard]] bool in_sparse_range(Key const key) const
         {
            return key < sparse_.size();
         }

         [[nodiscard]] bool in_dense_range(DataIndex const data_index) const
         {
            return data_index < dense_.size();
         }

         [[nodiscard]] bool naive_contains(Key const key) const
         {
            return sparse_[key] not_eq UNUSED_DATA_INDEX;
         }

         [[nodiscard]] Data& naive_find(Key const key)
         {
            return dense_data_[sparse_[key]];
         }

         template <typename... ArgumentTypes>
            requires std::constructible_from<Data, ArgumentTypes...>
         Data& naive_insert(Key const key, ArgumentTypes&&... arguments)
         {
            sparse_[key] = dense_.size();
            dense_.emplace_back(key);

            return dense_data_.emplace_back(std::forward<ArgumentTypes>(arguments)...);
         }

         void naive_move(Key const key, DataIndex const where)
         {
            Key const other_key{ dense_[where] };

            std::swap(dense_[sparse_[key]], dense_[where]);
            std::swap(dense_data_[sparse_[key]], dense_data_[where]);

            std::swap(sparse_[key], sparse_[other_key]);
         }

         std::vector<DataIndex> sparse_{};
         std::vector<Key> dense_{};
         std::vector<Data> dense_data_{};
   };
}

#endif