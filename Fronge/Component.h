#pragma once

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

		const GameObject& getParentingGameObject() const;

	private:
		Component(const Component&) = delete;
		Component(Component&&) noexcept = delete;

		Component& operator=(const Component&) = delete;
		Component& operator=(Component&&) noexcept = delete;

		const GameObject& m_ParentingGameObject;
	};
}

#ifndef fro_GENERATED_COMPONENT_BODY
#define fro_GENERATED_COMPONENT_BODY(identifier, ...)\
namespace fro\
{\
	class identifier final : public Component\
	{\
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
		identifier& operator=(identifier&&) noexcept = delete;\
\
		__VA_ARGS__\
	};\
}
#endif

#ifndef fro_GENERATED_COMPONENT_CONSTRUCTOR
#define fro_GENERATED_COMPONENT_CONSTRUCTOR(identifier, ...)\
fro::identifier::identifier(const GameObject& parentingGameObject) :\
	Component(parentingGameObject)\
{\
	__VA_ARGS__\
}
#endif

#ifndef fro_GENERATED_COMPONENT_DESTRUCTOR
#define fro_GENERATED_COMPONENT_DESTRUCTOR(identifier, ...)\
fro::identifier::~identifier()\
{\
	__VA_ARGS__\
}
#endif