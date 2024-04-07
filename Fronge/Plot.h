#pragma once

#include "GUI.h"

#include <deque>
#include <vector>
#include <chrono>
#include <algorithm>
#include <numeric>

namespace fro
{
	class Plot final : public GUI
	{
		fro_GENERATED_GUI_BODY(Plot)

	private:
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
		std::vector<double> measure(std::vector<DataType>& vData)
		{
			std::vector<double> vResults{};

			for (int stepSize{ 1 }; stepSize <= 1024; stepSize *= 2)
			{
				std::deque<std::chrono::milliseconds> dSamples(m_AmountOfSamples);

				for (auto& sample : dSamples)
				{
					const auto beginTimePoint{ std::chrono::steady_clock::now() };

					for (size_t index{}; index < vData.size(); index += stepSize)
						vData[index] *= 2;

					const auto endTimePoint{ std::chrono::steady_clock::now() };

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