#include "GUI.h"

#include "GameObject.h"

#pragma region Constructors/Destructor
fro::GUI::GUI(Reference<GameObject> const parentingGameObject)
	: Component(parentingGameObject)
{
}
#pragma endregion Constructors/Destructor