#include "State.h"

#pragma region Constructors/Destructor
fro::State::State(Reference<GameObject> const parentingGameObject)
	: Component(parentingGameObject)
{
}
#pragma endregion Constructors/Destructor



#pragma region PublicMethods
fro::Reference<fro::State> fro::State::fixedUpdate(float const)
{
	return {};
}

fro::Reference<fro::State> fro::State::update(float const)
{
	return {};
}

fro::Reference<fro::State> fro::State::lateUpdate(float const)
{
	return {};
}

void fro::State::enter(Reference<State> const)
{
}

void fro::State::exit(Reference<State> const)
{
}
#pragma endregion PublicMethods