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

   template <typename Key, SparseSetStorable Data>
   class SparseSet final
   {
      public:
         using SimpleKey = std::size_t;
         using DataIndex = SimpleKey;

         static DataIndex constexpr UNUSED_DATA_INDEX{ std::numeric_limits<DataIndex>::max() };

         SparseSet(std::function<SimpleKey(Key const&)> simple_key_converter,
            std::function<SimpleKey(Data const&)> simple_key_extractor,
            std::function<void(Key const&, Data&)> data_key_updater,
            std::function<void(Data&)> data_key_eraser)
            : simple_key_converter_{ std::move(simple_key_converter) }
            , simple_key_extractor_{ std::move(simple_key_extractor) }
            , data_key_updater_{ std::move(data_key_updater) }
            , data_key_eraser_{ std::move(data_key_eraser) }
         {
         }

         SparseSet(SparseSet const&) = default;
         SparseSet(SparseSet&&) noexcept = default;

         ~SparseSet() = default;

         SparseSet& operator=(SparseSet const&) = default;
         SparseSet& operator=(SparseSet&&) noexcept = default;

         template <typename... Arguments>
            requires std::constructible_from<Data, Arguments...>
         Data& insert(Key const& key, Arguments&&... arguments)
         {
            SimpleKey const simple_key{ simple_key_converter_(key) };

            if (not in_sparse_range(simple_key))
               sparse_.resize(key + 1, UNUSED_DATA_INDEX);
            else if (naive_contains(simple_key))
               return naive_find(simple_key);

            return naive_insert(key, simple_key, std::forward<Arguments>(arguments)...);
         }

         std::optional<Data> erase(Key const key)
         {
            SimpleKey const simple_key{ simple_key_converter_(key) };

            if (not simple_contains(simple_key))
               return std::nullopt;

            naive_move(simple_key, dense_.size() - 1);
            sparse_[simple_key] = UNUSED_DATA_INDEX;

            std::optional data{ std::move(dense_.back()) };
            dense_.pop_back();

            data_key_eraser_(*data);

            return data;
         }

         void clear()
         {
            sparse_.clear();
            dense_.clear();
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
         }

         void shrink()
         {
            shrink_sparse();
            shrink_dense();
         }

         bool move(Key const& key, DataIndex const where)
         {
            SimpleKey const simple_key{ simple_key_converter_(key) };

            if (not in_dense_range(where) or not simple_contains(simple_key) or sparse_[simple_key] == where)
               return false;

            naive_move(simple_key, where);
            return true;
         }

         [[nodiscard]] Data* find(Key const& key)
         {
            SimpleKey const simple_key{ simple_key_converter_(key) };

            if (not simple_contains(simple_key))
               return nullptr;

            return &naive_find(simple_key);
         }

         [[nodiscard]] Data const* find(Key const& key) const
         {
            SimpleKey const simple_key{ simple_key_converter_(key) };

            if (not simple_contains(simple_key))
               return nullptr;

            return &naive_find(simple_key);
         }

         [[nodiscard]] bool contains(Key const& key) const
         {
            SimpleKey const simple_key{ simple_key_converter_(key) };
            return simple_contains(simple_key);
         }

         [[nodiscard]] std::size_t sparse_capacity() const
         {
            return sparse_.capacity();
         }

         [[nodiscard]] std::size_t dense_capacity() const
         {
            return dense_.capacity();
         }

         [[nodiscard]] std::span<DataIndex> sparse()
         {
            return sparse_;
         }

         [[nodiscard]] std::span<DataIndex const> sparse() const
         {
            return sparse_;
         }

         [[nodiscard]] std::span<Data> dense()
         {
            return dense_;
         }

         [[nodiscard]] std::span<Data const> dense() const
         {
            return dense_;
         }

      private:
         [[nodiscard]] bool simple_contains(SimpleKey const simple_key) const
         {
            return in_sparse_range(simple_key) and naive_contains(simple_key);
         }

         template <typename... ArgumentTypes>
            requires std::constructible_from<Data, ArgumentTypes...>
         Data& naive_insert(Key const& key, SimpleKey const simple_key, ArgumentTypes&&... arguments)
         {
            sparse_[simple_key] = dense_.size();

            Data& data{ dense_.emplace_back(std::forward<ArgumentTypes>(arguments)...) };
            data_key_updater_(key, data);

            return data;
         }

         void naive_move(SimpleKey const simple_key, DataIndex const where)
         {
            SimpleKey const other_simple_key{ simple_key_extractor_(dense_[where]) };

            std::swap(dense_[sparse_[simple_key]], dense_[where]);
            std::swap(sparse_[simple_key], sparse_[other_simple_key]);
         }

         [[nodiscard]] Data& naive_find(SimpleKey const simple_key)
         {
            return dense_[sparse_[simple_key]];
         }

         [[nodiscard]] Data const& naive_find(SimpleKey const simple_key) const
         {
            return dense_[sparse_[simple_key]];
         }

         [[nodiscard]] bool naive_contains(SimpleKey const simple_key) const
         {
            return sparse_[simple_key] not_eq UNUSED_DATA_INDEX;
         }

         [[nodiscard]] bool in_sparse_range(SimpleKey const simple_key) const
         {
            return simple_key < sparse_.size();
         }

         [[nodiscard]] bool in_dense_range(DataIndex const data_index) const
         {
            return data_index < dense_.size();
         }

         std::function<SimpleKey(Key const&)> simple_key_converter_;
         std::function<SimpleKey(Data const&)> simple_key_extractor_;
         std::function<void(Key const&, Data&)> data_key_updater_;
         std::function<void(Data&)> data_key_eraser_;

         std::vector<DataIndex> sparse_{};
         std::vector<Data> dense_{};
   };
}

#endif