#include "NPC.h"

#include <algorithm>

#pragma region Constructors/Destructor
NPC::NPC(std::string name)
	: m_Name(std::move(name))
{
}
#pragma endregion Constructors/Destructor



#pragma region PublicMethods
void NPC::setHealth(int const health)
{
	m_Health = std::clamp(health, 0, 100);
}

std::string_view NPC::getName() const
{
	return m_Name;
}

int NPC::getHealth() const
{
	return m_Health;
}
#pragma endregion PublicMethods