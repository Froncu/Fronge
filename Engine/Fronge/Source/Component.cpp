#include "Component.h"

#include "GameObject.h"

#pragma region Constructors/Destructor
fro::Component::Component(Reference<GameObject> const parentingGameObject)
	: m_ParentingGameObject{ parentingGameObject }
{
}
#pragma endregion Constructors/Destructor