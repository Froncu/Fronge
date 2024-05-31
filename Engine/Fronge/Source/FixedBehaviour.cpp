#include "FixedBehaviour.h"

#include "GameObject.h"

#pragma region Constructors/Destructor
fro::FixedBehaviour::FixedBehaviour(Reference<GameObject> const parentingGameObject)
	: Component(std::move(parentingGameObject))
{
}
#pragma endregion Constructors/Destructor