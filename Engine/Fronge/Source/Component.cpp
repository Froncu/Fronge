#include "Component.h"

#include "GameObject.h"

#pragma region Constructors/Destructor
fro::Component::Component(Reference<GameObject> const parentingGameObject)
	: parentingGameObject{ parentingGameObject }
{
}
#pragma endregion Constructors/Destructor



#pragma region PublicMethods
void fro::Component::awake()
{
}
#pragma endregion PublicMethods