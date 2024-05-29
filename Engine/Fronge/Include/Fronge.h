#if not defined fro_FRONGE_H
#define fro_FRONGE_H

#include "Singleton.hpp"

namespace fro
{
	class Fronge final : public Singleton<Fronge>
	{
		friend class Singleton<Fronge>;

	public:
		virtual ~Fronge() override;

		int run() const;

		void setFixedFrameRate(int const fixedFrameRate);

	private:
		Fronge();
		Fronge(Fronge const&) = delete;
		Fronge(Fronge&&) noexcept = delete;

		Fronge& operator=(Fronge const&) = delete;
		Fronge& operator=(Fronge&&) noexcept = delete;

		float m_FixedDeltaSeconds{ 1 / 30.0f };
	};
}

#endif