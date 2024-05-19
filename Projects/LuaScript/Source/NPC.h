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

	void setHealth(int const health);

	std::string_view getName() const;
	int getHealth() const;

private:
	std::string m_Name;
	int m_Health{ 100 };
};

#endif