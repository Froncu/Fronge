#include "GUI.h"

#include "GameObject.h"

#pragma region Constructors/Destructor
fro::GUI::GUI(Reference<GameObject> const parentingGameObject)
	: Component(std::move(parentingGameObject))
{
}
#pragma endregion Constructors/Destructor