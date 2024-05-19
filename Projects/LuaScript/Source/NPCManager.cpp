#include "NPCManager.h"

#include <algorithm>
#include <format>
#include <iostream>

#pragma region PublicMethods
NPCManager& NPCManager::getInstance()
{
	static NPCManager instance{};
	return instance;
}

void NPCManager::addNPC(std::string name)
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

void NPCManager::setHealth(std::string_view const NPCName, int health)
{
	auto const iNPC
	{
		std::find_if(m_vNPCs.begin(), m_vNPCs.end(),
		[NPCName](NPC const& containedNPC)
		{
			return containedNPC.getName() == NPCName;
		})
	};

	if (iNPC not_eq m_vNPCs.end())
		iNPC->setHealth(health);
}

void NPCManager::dump() const
{
	std::cout << std::format("The {} available NPC's:\n", m_vNPCs.size());

	for (const auto& npc : m_vNPCs)
		std::cout << std::format("{} with {} health\n", npc.getName(), npc.getHealth());
}
#pragma endregion PublicMethods