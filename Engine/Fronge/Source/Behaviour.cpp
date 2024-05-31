#include "Behaviour.h"

#include "GameObject.h"

#pragma region Constructors/Destructor
fro::Behaviour::Behaviour(Reference<GameObject> const parentingGameObject)
	: Component(std::move(parentingGameObject))
{
}
#pragma endregion Constructors/Destructor