#include "NPCManager.h"

#include <algorithm>
#include <iostream>

#pragma region PublicMethods
NPCManager& NPCManager::getInstance()
{
	static NPCManager instance{};
	return instance;
}

void NPCManager::addNPC(std::string const name)
{
	m_vNPCs.emplace_back(std::move(name));
}

void NPCManager::removeNPC(std::string_view name)
{
	auto const iNewEnd
	{
		std::remove_if(m_vNPCs.begin(), m_vNPCs.end(),
		[name](NPC const& containedNPC)
		{
			return containedNPC.getName() == name;
		})
	};

	m_vNPCs.erase(iNewEnd, m_vNPCs.end());
}

void NPCManager::dump() const
{
	std::cout << "Available NPC's:" << std::endl;
	for (const auto& npc : m_vNPCs)
		std::cout << "NPC " << npc.getName() << std::endl;

	std::cout << "NPC count:" << m_vNPCs.size() << std::endl;
}
#pragma endregion PublicMethods