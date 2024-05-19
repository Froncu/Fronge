#if not defined fro_NPC_H
#define fro_NPC_H

#include <string>

class NPC final
{
public:
	explicit NPC(std::string name);
	NPC(NPC const&) = default;
	NPC(NPC&&) noexcept = default;

	~NPC() = default;

	NPC& operator=(NPC const&) = default;
	NPC& operator=(NPC&&) noexcept = default;

	std::string_view getName() const;

private:
	std::string m_Name;
};

#endif