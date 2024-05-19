#include "NPC.h"

#pragma region Constructors/Destructor
NPC::NPC(std::string name)
	: m_Name(std::move(name))
{
}
#pragma endregion Constructors/Destructor



#pragma region PublicMethods
std::string_view NPC::getName() const
{
	return m_Name;
}
#pragma endregion PublicMethods