#include "GameObject.h"

#pragma region StaticData
std::size_t fro::GameObject::NEXT_ID{};
#pragma endregion StaticData



#pragma region Operators
fro::GameObject::operator std::size_t& ()
{
	return m_ID;
}

fro::GameObject::operator std::size_t const& () const
{
	return m_ID;
}
#pragma endregion Operators