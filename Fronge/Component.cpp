#include "Component.h"

#include "GameObject.h"

#pragma region Constructors/Destructor
fro::Component::Component(const GameObject& parentingGameObject) :
	m_ParentingGameObject{ parentingGameObject }
{
}
#pragma endregion Constructors/Destructor



#pragma region ProtecedMethods
const fro::GameObject& fro::Component::getParentingGameObject() const
{
	return m_ParentingGameObject;
}
#pragma endregion ProtecedMethods