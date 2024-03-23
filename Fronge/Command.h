#pragma once

namespace fro
{
	class Command
	{
		friend class InputManager;

	public:
		virtual ~Command() = default;

	protected:
		Command() = default;

	private:
		Command(const Command&) = delete;
		Command(Command&&) noexcept = delete;

		Command& operator=(const Command&) = delete;
		Command& operator=(Command&&) noexcept = delete;

		virtual void operator()() = 0;
	};
}

#ifndef fro_GENERATED_COMMAND_BODY
#define fro_GENERATED_COMMAND_BODY(identifier)\
friend class InputManager;\
\
public:\
	virtual ~identifier() override; \
\
private:\
	identifier();\
	identifier(const identifier&) = delete;\
	identifier(identifier&&) noexcept = delete;\
\
	identifier& operator=(const identifier&) = delete;\
	identifier& operator=(identifier&&) noexcept = delete;\
\
	virtual void operator()();
#endif

#ifndef fro_GENERATED_COMMAND_CONSTRUCTOR
#define fro_GENERATED_COMMAND_CONSTRUCTOR(identifier)\
fro::identifier::identifier()
#endif

#ifndef fro_GENERATED_COMMAND_DESTRUCTOR
#define fro_GENERATED_COMMAND_DESTRUCTOR(identifier)\
fro::identifier::~identifier()
#endif

#ifndef fro_GENERATED_COMMAND_OPERATOR
#define fro_GENERATED_COMMAND_OPERATOR(identifier)\
void fro::identifier::operator()()
#endif