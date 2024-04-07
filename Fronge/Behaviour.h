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
#define fro_GENERATED_BEHAVIOUR_BODY(identifier)\
fro_GENERATED_COMPONENT_BODY(identifier)\
\
private:\
	virtual void update() override;
#endif

#ifndef fro_GENERATED_BEHAVIOUR_CONSTRUCTOR
#define fro_GENERATED_BEHAVIOUR_CONSTRUCTOR(identifier)\
fro::identifier::identifier(const GameObject& parentingGameObject)\
	: Behaviour(parentingGameObject)
#endif

#ifndef fro_GENERATED_BEHAVIOUR_DESTRUCTOR
#define fro_GENERATED_BEHAVIOUR_DESTRUCTOR(identifier)\
fro_GENERATED_COMPONENT_DESTRUCTOR(identifier)
#endif

#ifndef fro_GENERATED_BEHAVIOUR_UPDATE
#define fro_GENERATED_BEHAVIOUR_UPDATE(identifier)\
void fro::identifier::update()
#endif