#pragma once

#include "Defines.hpp"

#include <functional>

namespace fro
{
	template<typename PrimaryDataType, typename DerivedDataType = PrimaryDataType>
	class DirtyData final
	{
	public:
		DirtyData(std::function<void(PrimaryDataType&, DerivedDataType&)> derivedDataCalculator) :
			m_DerivedDataCalculator{ derivedDataCalculator }
		{
		};

		DirtyData(const DirtyData&) = default;
		DirtyData(DirtyData&&) noexcept = default;

		~DirtyData() = default;

		DirtyData& operator=(const DirtyData&) = default;
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

		fro_NODISCARD const DerivedDataType& getPrimaryData() const
		{
			return m_PrimaryData;
		}

		fro_NODISCARD const PrimaryDataType& getDerivedData()
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
		std::function<void(const PrimaryDataType&, DerivedDataType&)> m_DerivedDataCalculator;

		bool m_IsDerivedDataDirty{ true };
	};
}