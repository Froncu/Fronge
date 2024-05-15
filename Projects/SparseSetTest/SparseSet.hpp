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
		SparseSet() = default;
		SparseSet(SparseSet const&) = default;
		SparseSet(SparseSet&&) noexcept = default;

		~SparseSet() = default;

		SparseSet& operator=(SparseSet const&) = default;
		SparseSet& operator=(SparseSet&&) noexcept = default;
		DataType& operator[](std::size_t const ID)
		{
			if (contains(ID))
				return naiveFind(ID);

			return naiveInsert(ID, {});
		}

		void clear()
		{
			m_vSparse.clear();
			m_vDense.clear();
		}

		DataType* insert(std::size_t const ID, DataType data = {})
		{
			if (not inRange(ID))
				m_vSparse.resize(ID + 1, UNUSED_DATA_INDEX);

			else if (naiveContains(ID))
				return nullptr;

			return &naiveInsert(ID, data);
		}

		bool contains(std::size_t const ID)
		{
			return inRange(ID) and naiveContains(ID);
		}

		DataType* find(std::size_t const ID)
		{
			if (not contains(ID))
				return nullptr;

			return &naiveFind(ID);
		}

		bool remove(std::size_t const ID)
		{
			if (not contains(ID))
				return false;

			std::size_t lastDense{ m_vDense.back() };
			std::swap(naiveFind(ID), m_vDense.back());

			m_vSparse[lastDense] = m_vSparse[ID];
			m_vSparse[ID] = UNUSED_DATA_INDEX;

			m_vDense.pop_back();

			return true;
		}

	private:
		static std::size_t constexpr UNUSED_DATA_INDEX{ std::numeric_limits<std::size_t>::max() };

		bool inRange(std::size_t const ID)
		{
			return ID < m_vSparse.size();
		}

		bool naiveContains(std::size_t const ID)
		{
			return m_vSparse[ID] not_eq UNUSED_DATA_INDEX;
		}

		DataType& naiveFind(std::size_t const ID)
		{
			return m_vDense[m_vSparse[ID]];
		}

		DataType& naiveInsert(std::size_t const ID, DataType data)
		{
			m_vSparse[ID] = m_vDense.size();

			static_cast<std::size_t&>(data) = ID;

			m_vDense.push_back(std::move(data));
			return m_vDense.back();
		}

		std::vector<std::size_t> m_vSparse{};
		std::vector<DataType> m_vDense{};
	};
}

#endif