#include "Component.h"

#pragma region Constructors/Destructor
fro::Component::Component(GameObject const& parentingGameObject)
	: m_ParentingGameObject{ parentingGameObject }
{
}
#pragma endregion Constructors/Destructor



#pragma region PublicMethods
fro::GameObject const& fro::Component::getParentingGameObject() const
{
	return m_ParentingGameObject;
}
#pragma endregion PublicMethods