#pragma once

#include "Defines.hpp"

namespace fro
{
	class GameObject;

	class Component
	{
		friend GameObject;

	public:
		virtual ~Component() = default;

	protected:
		Component(const GameObject& parentingGameObject);

		fro_NODISCARD_GETTER const GameObject& getParentingGameObject() const;

	private:
		Component(const Component&) = delete;
		Component(Component&&) noexcept = delete;

		Component& operator=(const Component&) = delete;
		Component& operator=(Component&&) noexcept = delete;

		const GameObject& m_ParentingGameObject;
	};
}

#ifndef fro_GENERATED_COMPONENT_BODY
#define fro_GENERATED_COMPONENT_BODY(identifier)\
friend GameObject;\
\
public:\
	virtual ~identifier() override; \
\
private:\
	identifier(const GameObject& parentingGameObject);\
	identifier(const identifier&) = delete;\
	identifier(identifier&&) noexcept = delete;\
\
	identifier& operator=(const identifier&) = delete;\
	identifier& operator=(identifier&&) noexcept = delete;
#endif

#ifndef fro_GENERATED_COMPONENT_CONSTRUCTOR
#define fro_GENERATED_COMPONENT_CONSTRUCTOR(identifier)\
fro::identifier::identifier(const GameObject& parentingGameObject)\
	: Component(parentingGameObject)
#endif

#ifndef fro_GENERATED_COMPONENT_DESTRUCTOR
#define fro_GENERATED_COMPONENT_DESTRUCTOR(identifier)\
fro::identifier::~identifier()
#endif