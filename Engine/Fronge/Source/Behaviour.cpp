#include "Behaviour.h"

#include "GameObject.h"

#pragma region Constructors/Destructor
fro::Behaviour::Behaviour(Reference<GameObject> const parentingGameObject)
	: Component(parentingGameObject)
{
}
#pragma endregion Constructors/Destructor



#pragma region PublicMethods
void fro::Behaviour::fixedUpdate(float const)
{
}

void fro::Behaviour::update(float const)
{
}

void fro::Behaviour::lateUpdate(float const)
{
}
#pragma endregion PublicMethods