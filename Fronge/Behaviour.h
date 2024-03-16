#pragma once

#include "Component.h"

namespace fro
{
	class GameObject;

	class Behaviour : public Component
	{
		friend GameObject;

	public:
		virtual ~Behaviour() = default;

	protected:
		Behaviour(const GameObject& parentingGameObject);

	private:
		Behaviour(const Behaviour&) = delete;
		Behaviour(Behaviour&&) noexcept = delete;

		Behaviour& operator=(const Behaviour&) = delete;
		Behaviour& operator=(Behaviour&&) noexcept = delete;

		virtual void update() = 0;
	};
}

#ifndef fro_GENERATED_BEHAVIOUR_BODY
#define fro_GENERATED_BEHAVIOUR_BODY(identifier, ...)\
namespace fro\
{\
	class identifier final : public Behaviour\
	{\
		friend GameObject;\
\
	public:\
		virtual ~identifier() override; \
\
	private:\
		identifier(const GameObject& parentingGameObject);\
		identifier(const identifier&) = delete;\
		identifier(identifier&&) noexcept = delete;\
\
		identifier& operator=(const identifier&) = delete;\
		identifier& operator=(identifier&&) noexcept = delete;\
\
		virtual void update() override;\
\
		__VA_ARGS__\
	};\
}
#endif

#ifndef fro_GENERATED_BEHAVIOUR_CONSTRUCTOR
#define fro_GENERATED_BEHAVIOUR_CONSTRUCTOR(identifier, ...)\
fro::identifier::identifier(const GameObject& parentingGameObject) :\
	Behaviour(parentingGameObject)\
{\
	__VA_ARGS__\
}
#endif

#ifndef fro_GENERATED_BEHAVIOUR_DESTRUCTOR
#define fro_GENERATED_BEHAVIOUR_DESTRUCTOR(identifier, ...)\
fro_GENERATED_COMPONENT_DESTRUCTOR(identifier, __VA_ARGS__)
#endif

#ifndef fro_GENERATED_BEHAVIOUR_UPDATE
#define fro_GENERATED_BEHAVIOUR_UPDATE(identifier, body)\
void fro::identifier::update()\
{\
	body\
}
#endif