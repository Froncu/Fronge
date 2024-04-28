#if not defined fro_DIRTY_DATA_H
#define fro_DIRTY_DATA_H

#include "Defines.hpp"

#include <functional>

namespace fro
{
	template<typename PrimaryDataType, typename DerivedDataType = PrimaryDataType>
	class DirtyData final
	{
	public:
		DirtyData(std::function<void(PrimaryDataType const&, DerivedDataType&)> derivedDataCalculator)
			: m_DerivedDataCalculator{ derivedDataCalculator }
		{
		};

		DirtyData(DirtyData const&) = default;
		DirtyData(DirtyData&&) noexcept = default;

		~DirtyData() = default;

		DirtyData& operator=(DirtyData const&) = default;
		DirtyData& operator=(DirtyData&&) noexcept = default;

		void calculateDerivedData()
		{
			m_DerivedDataCalculator(m_PrimaryData, m_DerivedData);
		}

		void setPrimaryData(PrimaryDataType&& value)
		{
			m_PrimaryData = std::forward(value);
			m_IsDerivedDataDirty = true;
		}

		fro_NODISCARD DerivedDataType const& getPrimaryData() const
		{
			return m_PrimaryData;
		}

		fro_NODISCARD PrimaryDataType const& getDerivedData()
		{
			if (m_IsDerivedDataDirty)
				calculateDerivedData();

			return m_DerivedData;
		}

		fro_NODISCARD bool isDirty() const
		{
			return m_IsDerivedDataDirty;
		}

	private:
		PrimaryDataType m_PrimaryData;
		DerivedDataType m_DerivedData;
		std::function<void(PrimaryDataType const&, DerivedDataType&)> m_DerivedDataCalculator;

		bool m_IsDerivedDataDirty{ true };
	};
}

#endif