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

#ifndef fro_GENERATED_SINGLETON_BODY
#define fro_GENERATED_SINGLETON_BODY(identifier)\
friend class Singleton<identifier>;\
\
public:\
	virtual ~identifier() override;\
\
private:\
	identifier();\
	identifier(const identifier&) = delete;\
	identifier(identifier&&) noexcept = delete;\
\
	identifier& operator=(const identifier&) = delete;\
	identifier& operator=(identifier&&) noexcept = delete;
#endif

#ifndef fro_GENERATED_SINGLETON_CONSTRUCTOR
#define fro_GENERATED_SINGLETON_CONSTRUCTOR(identifier)\
fro::identifier::identifier()
#endif

#ifndef fro_GENERATED_SINGLETON_DESTRUCTOR
#define fro_GENERATED_SINGLETON_DESTRUCTOR(identifier)\
fro::identifier::~identifier()
#endif