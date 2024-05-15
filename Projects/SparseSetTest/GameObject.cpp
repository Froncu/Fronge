#include "GameObject.h"

#pragma region Constructors/Destructor
fro::GameObject::GameObject(std::size_t const ID)
	: m_ID{ ID }
{
}
#pragma endregion Constructors/Destructor



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