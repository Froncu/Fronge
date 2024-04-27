#if not defined fro_SINGLETON_H
#define fro_SINGLETON_H

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
		Singleton(Singleton const&) = delete;
		Singleton(Singleton&&) noexcept = delete;

		Singleton& operator=(Singleton const&) = delete;
		Singleton& operator=(Singleton&&) noexcept = delete;
	};
}

#endif