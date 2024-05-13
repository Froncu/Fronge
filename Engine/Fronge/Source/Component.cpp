#include "Component.h"

#pragma region Constructors/Destructor
fro::Component::Component(GameObject& parentingGameObject)
	: m_pParentingGameObject{ &parentingGameObject }
{
}
#pragma endregion Constructors/Destructor



#pragma region PublicMethods
fro::GameObject& fro::Component::getParentingGameObject() const
{
	return *m_pParentingGameObject;
}
#pragma endregion PublicMethods