// Written based on Michele "skypjack" Caini's
// blog posts about ECS -> https://skypjack.github.io/

#if not defined fro_SPARSE_SET_H
#define fro_SPARSE_SET_H

#include <optional>
#include <vector>

namespace fro
{
	template<typename DataType>
	class SparseSet final
	{
	public:
		using Key = std::size_t;
		using DataIndex = Key;
		static DataIndex constexpr UNUSED_DATA_INDEX{ std::numeric_limits<DataIndex>::max() };

		SparseSet() = default;
		SparseSet(SparseSet const&) = default;
		SparseSet(SparseSet&&) noexcept = default;

		~SparseSet() = default;

		SparseSet& operator=(SparseSet const&) = default;
		SparseSet& operator=(SparseSet&&) noexcept = default;
		DataType& operator[](Key const key)
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
			if (not inRange(key))
				m_vSparse.resize(key + 1, UNUSED_DATA_INDEX);

			else if (naiveContains(key))
				return nullptr;

			return &naiveInsert(key, std::move(data));
		}

		bool contains(Key const key)
		{
			return inRange(key) and naiveContains(key);
		}

		DataType* find(Key const key)
		{
			if (not contains(key))
				return nullptr;

			return &naiveFind(key);
		}

		bool remove(Key const key)
		{
			if (not contains(key))
				return false;

			Key const lastDataKey{ m_vDense.back() };
			std::swap(naiveFind(key), m_vDense.back());

			m_vSparse[lastDataKey] = m_vSparse[key];
			m_vSparse[key] = UNUSED_DATA_INDEX;

			m_vDense.pop_back();

			return true;
		}

	private:
		bool inRange(Key const key)
		{
			return key < m_vSparse.size();
		}

		bool naiveContains(Key const key)
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

		std::vector<DataIndex> m_vSparse{};
		std::vector<DataType> m_vDense{};
	};
}

#endif