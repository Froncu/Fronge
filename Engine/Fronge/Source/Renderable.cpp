#include "Renderable.h"

#include "GameObject.h"

#pragma region Constructors/Destructor
fro::Renderable::Renderable(Reference<GameObject> const parentingGameObject)
	: Component(std::move(parentingGameObject))
{
}
#pragma endregion Constructors/Destructor