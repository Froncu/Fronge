#pragma once

namespace fro
{
	template<typename InheritingType>
	class Singleton
	{
	public:
		Singleton() = default;

		virtual ~Singleton() = default;

		static InheritingType& getInstance()
		{
			static InheritingType instance{};

			return instance;
		};

	private:
		Singleton(const Singleton&) = delete;
		Singleton(Singleton&&) noexcept = delete;

		Singleton& operator=(const Singleton&) = delete;
		Singleton& operator=(Singleton&&) noexcept = delete;
	};
}