// Written based on Michele "skypjack" Caini's
// blog posts about ECS (https://skypjack.github.io/)
// as well as his EnTT library (https://github.com/skypjack/entt)

#if not defined SPARSE_SET_HPP
#define SPARSE_SET_HPP

#include "froch.hpp"

#include "Core.hpp"

namespace fro
{
	template<typename Type>
	concept SparseSetStorable =
		std::swappable<Type> and
		std::movable<Type>;

	template<SparseSetStorable DataType>
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
		FRO_NODISCARD std::pair<DataType&, bool> operator[](Key const key)
			requires std::default_initializable<DataType>
		{
			if (not inSparseRange(key))
				mSparse.resize(key + 1, UNUSED_DATA_INDEX);

			else if (naiveContains(key))
				return { naiveFind(key), false };

			return { naiveInsert(key), true };
		}

		template<typename... ArgumentTypes>
			requires std::constructible_from<DataType, ArgumentTypes...>
		DataType* insert(Key const key, ArgumentTypes&&... arguments)
		{
			if (not inSparseRange(key))
				mSparse.resize(key + 1, UNUSED_DATA_INDEX);

			else if (naiveContains(key))
				return nullptr;

			return &naiveInsert(key, std::forward<ArgumentTypes>(arguments)...);
		}

		std::optional<DataType> erase(Key const key)
		{
			if (not contains(key))
				return std::nullopt;

			naiveMove(key, mDense.size() - 1);
			mSparse[key] = UNUSED_DATA_INDEX;
			mDense.pop_back();

			std::optional data{ std::move(mDenseData.back()) };
			mDenseData.pop_back();

			return data;
		}

		FRO_NODISCARD DataType* find(Key const key)
		{
			if (not contains(key))
				return nullptr;

			return &naiveFind(key);
		}

		void clear()
		{
			mSparse.clear();
			mDense.clear();
			mDenseData.clear();
		}

		void shrinkSparse()
		{
			auto const lastUsedKey
			{
				std::find_if(mSparse.rbegin(), mSparse.rend(),
				[](Key const key)
				{
					return key not_eq UNUSED_DATA_INDEX;
				}).base()
			};

			mSparse.erase(lastUsedKey, mSparse.end());
			mSparse.shrink_to_fit();
		}

		void shrinkDense()
		{
			mDense.shrink_to_fit();
			mDenseData.shrink_to_fit();
		}

		void shrink()
		{
			shrinkSparse();
			shrinkDense();
		}

		bool move(Key const key, DataIndex const where)
		{
			if (not inDenseRange(where) or not contains(key))
				return false;

			naiveMove(key, where);
			return true;
		}

		FRO_NODISCARD bool contains(Key const key) const
		{
			return inSparseRange(key) and naiveContains(key);
		}

		FRO_NODISCARD std::size_t sparseSize() const
		{
			return mSparse.size();
		}

		FRO_NODISCARD std::size_t sparseCapacity() const
		{
			return mSparse.capacity();
		}

		FRO_NODISCARD std::size_t denseSize() const
		{
			return mDenseData.size();
		}

		FRO_NODISCARD std::size_t denseCapacity() const
		{
			return mDenseData.capacity();
		}

	private:
		FRO_NODISCARD bool inSparseRange(Key const key) const
		{
			return key < mSparse.size();
		}

		FRO_NODISCARD bool inDenseRange(DataIndex const dataIndex) const
		{
			return dataIndex < mDense.size();
		}

		FRO_NODISCARD bool naiveContains(Key const key) const
		{
			return mSparse[key] not_eq UNUSED_DATA_INDEX;
		}

		FRO_NODISCARD DataType& naiveFind(Key const key)
		{
			return mDenseData[mSparse[key]];
		}

		template<typename... ArgumentTypes>
			requires std::constructible_from<DataType, ArgumentTypes...>
		DataType& naiveInsert(Key const key, ArgumentTypes&&... arguments)
		{
			mSparse[key] = mDense.size();
			mDense.emplace_back(key);

			return mDenseData.emplace_back(std::forward<ArgumentTypes>(arguments)...);
		}

		void naiveMove(Key const key, DataIndex const where)
		{
			Key const otherKey{ mDense[where] };

			std::swap(mDense[mSparse[key]], mDense[where]);
			std::swap(mDenseData[mSparse[key]], mDenseData[where]);

			std::swap(mSparse[key], mSparse[otherKey]);
		}

		std::vector<DataIndex> mSparse{};
		std::vector<Key> mDense{};
		std::vector<DataType> mDenseData{};
	};
}

#endif