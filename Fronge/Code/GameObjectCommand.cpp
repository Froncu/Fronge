#include "GameObjectCommand.h"

#pragma region Constructors/Destructor
fro::GameObjectCommand::GameObjectCommand(GameObject const& commandedGameObject)
	: m_CommandedGameObject{ commandedGameObject }
{
}
#pragma endregion Constructors/Destructor



#pragma region PublicMethods
fro_NODISCARD fro::GameObject const& fro::GameObjectCommand::getCommandedGameObject() const
{
	return m_CommandedGameObject;
}
#pragma endregion PublicMethods