#include "BaseReferencable.h"

#include "BaseReference.h"

#pragma region Constructors/Destructor
fro::BaseReferencable::BaseReferencable(BaseReferencable&& other) noexcept
	: m_spReferences{ std::move(other.m_spReferences) }
{
	for (BaseReference* const pReference : m_spReferences)
		pReference->m_pReferencable = this;
}

fro::BaseReferencable::~BaseReferencable()
{
	for (BaseReference* const pReference : m_spReferences)
		pReference->m_pReferencable = nullptr;
}
#pragma endregion Constructors/Destructor



#pragma region Operators
fro::BaseReferencable& fro::BaseReferencable::operator=(BaseReferencable&& other) noexcept
{
	for (BaseReference* const pReference : other.m_spReferences)
		pReference->m_pReferencable = this;

	m_spReferences.merge(other.m_spReferences);

	return *this;
}
#pragma endregion Operators