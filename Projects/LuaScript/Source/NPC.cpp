#include "NPC.h"

#include "Console.hpp"

#include <algorithm>
#include <format>

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

#if not defined NDEBUG
	auto const message{ std::format("{}'s health changed to {} health!", m_Name, m_Health) };
	fro::Console::getInstance().log(message, fro::Console::BackgroundColor::darkerBlue);
#endif
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