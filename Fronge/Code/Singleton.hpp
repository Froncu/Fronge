#pragma once

#include "Defines.hpp"

namespace fro
{
	template<typename InheritingType>
	class Singleton
	{
	public:
		fro_NODISCARD static InheritingType& getInstance()
		{
			static InheritingType instance{};

			return instance;
		};

	protected:
		Singleton() = default;

		virtual ~Singleton() = default;

	private:
		Singleton(const Singleton&) = delete;
		Singleton(Singleton&&) noexcept = delete;

		Singleton& operator=(const Singleton&) = delete;
		Singleton& operator=(Singleton&&) noexcept = delete;
	};
}