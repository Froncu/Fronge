// Written based on Michele "skypjack" Caini's
// blog posts about ECS (https://skypjack.github.io/)
// as well as his EnTT library (https://github.com/skypjack/entt)

#if not defined fro_SPARSE_SET_H
#define fro_SPARSE_SET_H

#include <limits>
#include <vector>

namespace fro
{
	template<typename DataType>
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
		SparseSet& operator[](Key const key)
		{
			if (contains(key))
				return naiveFind(key);

			return naiveInsert(key, {});
		}

		void clear()
		{
			m_vSparse.clear();
			m_vDense.clear();
		}

		DataType* insert(Key const key, DataType data = {})
		{
			if (not inSparseRange(key))
				m_vSparse.resize(key + 1, UNUSED_DATA_INDEX);

			else if (naiveContains(key))
				return nullptr;

			return &naiveInsert(key, std::move(data));
		}

		bool contains(Key const key) const
		{
			return inSparseRange(key) and naiveContains(key);
		}

		DataType* find(Key const key)
		{
			if (not contains(key))
				return nullptr;

			return &naiveFind(key);
		}

		bool erase(Key const key)
		{
			if (not contains(key))
				return false;

			naiveSwap(key, m_vDense.size() - 1);
			m_vSparse[key] = UNUSED_DATA_INDEX;
			m_vDense.pop_back();

			return true;
		}

		bool swap(Key const key, DataIndex const where)
		{
			if (not inDenseRange(where) or not contains(key))
				return false;

			naiveSwap(key, where);
			return true;
		}

		std::size_t size() const
		{
			return m_vDense.size();
		}

	private:
		bool inSparseRange(Key const key) const
		{
			return key < m_vSparse.size();
		}

		bool inDenseRange(DataIndex const dataIndex) const
		{
			return dataIndex < m_vDense.size();
		}

		bool naiveContains(Key const key) const
		{
			return m_vSparse[key] not_eq UNUSED_DATA_INDEX;
		}

		DataType& naiveFind(Key const key)
		{
			return m_vDense[m_vSparse[key]];
		}

		DataType& naiveInsert(Key const key, DataType data)
		{
			m_vSparse[key] = m_vDense.size();

			static_cast<Key&>(data) = key;

			m_vDense.push_back(std::move(data));
			return m_vDense.back();
		}

		void naiveSwap(Key const key, DataIndex const where)
		{
			Key const otherKey{ m_vDense[where] };
			std::swap(naiveFind(key), m_vDense[where]);
			std::swap(m_vSparse[key], m_vSparse[otherKey]);
		}

		std::vector<DataIndex> m_vSparse{};
		std::vector<DataType> m_vDense{};
	};
}

#endif