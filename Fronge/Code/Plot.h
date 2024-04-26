#pragma once

#include "GUI.h"
#include "Defines.hpp"

#include <vector>
#include <deque>
#include <chrono>
#include <algorithm>
#include <numeric>

namespace fro
{
	class Plot final : public GUI
	{
	public:
		Plot(GameObject const& parentingGameObject);

		virtual ~Plot() override = default;
		
		virtual void display() override;

	private:
		Plot(Plot const&) = delete;
		Plot(Plot&&) noexcept = delete;

		Plot& operator=(Plot const&) = delete;
		Plot& operator=(Plot&&) noexcept = delete;

		struct Transform
		{
			float matrix[16]
			{
				1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
			};
		};

		class GameObject3D
		{
		public:
			GameObject3D& operator*=(int value)
			{
				ID *= value;
				return *this;
			}

			int ID{};
			Transform transform{};
		};

		class GameObject3DAlt
		{
		public:
			GameObject3DAlt& operator*=(int value)
			{
				ID *= value;
				return *this;
			}

			int ID{};
			Transform* pTransform{};
		};

		template<typename DataType>
		fro_NODISCARD std::vector<double> measure(std::vector<DataType>& vData) const
		{
			std::vector<double> vResults{};

			for (int stepSize{ 1 }; stepSize <= 1024; stepSize *= 2)
			{
				std::deque<std::chrono::milliseconds> dSamples(m_AmountOfSamples);

				for (auto& sample : dSamples)
				{
					auto const beginTimePoint{ std::chrono::steady_clock::now() };

					for (std::size_t index{}; index < vData.size(); index += stepSize)
						vData[index] *= 2;

					auto const endTimePoint{ std::chrono::steady_clock::now() };

					sample = std::chrono::duration_cast<std::chrono::milliseconds>(endTimePoint - beginTimePoint);
				}

				std::sort(dSamples.begin(), dSamples.end());
				dSamples.pop_front();
				dSamples.pop_back();

				vResults.push_back
				(
					static_cast<double>((std::reduce(dSamples.begin(), dSamples.end()) / dSamples.size()).count())
				);
			}

			return vResults;
		}

		int m_AmountOfSamplesInteger{ 10 };
		int m_AmountOfSamples{ 10 };

		std::vector<int> m_vIntegers;
		std::vector<GameObject3D> m_vGameObjects3D;
		std::vector<GameObject3DAlt> m_vGameObjects3DAlt;

		std::vector<double> m_vIntegersResults;
		std::vector<double> m_vGameObject3DResults;
		std::vector<double> m_vGameObject3DAltResults;
	};
}