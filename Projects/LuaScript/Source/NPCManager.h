#if not defined fro_NPC_MANAGER_H
#define fro_NPC_MANAGER_H

#include "NPC.h"

#include <vector>

class NPCManager final
{
public:
	static NPCManager& getInstance();

	NPCManager() = default;
	NPCManager(NPCManager const&) = default;
	NPCManager(NPCManager&&) noexcept = default;

	~NPCManager() = default;

	NPCManager& operator=(NPCManager const&) = default;
	NPCManager& operator=(NPCManager&&) noexcept = default;

	void addNPC(std::string const name);
	void removeNPC(std::string_view const name);
	void dump() const;

private:
	std::vector<NPC> m_vNPCs{};
};

#endif