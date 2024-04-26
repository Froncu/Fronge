#include "GameObjectCommand.h"

#pragma region Constructors/Destructor
fro::GameObjectCommand::GameObjectCommand(const GameObject& commandedGameObject)
	: m_CommandedGameObject{ commandedGameObject }
{
}
#pragma endregion Constructors/Destructor



#pragma region PublicMethods
fro_NODISCARD const fro::GameObject& fro::GameObjectCommand::getCommandedGameObject() const
{
	return m_CommandedGameObject;
}
#pragma endregion PublicMethods