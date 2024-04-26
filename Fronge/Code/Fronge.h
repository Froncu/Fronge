#pragma once

#include "Singleton.hpp"

namespace fro
{
	class Fronge final : public Singleton<Fronge>
	{
	public:
		Fronge();

		virtual ~Fronge() override;

		int run();

	private:
		Fronge(Fronge const&) = delete;
		Fronge(Fronge&&) noexcept = delete;

		Fronge& operator=(Fronge const&) = delete;
		Fronge& operator=(Fronge&&) noexcept = delete;
	};
}