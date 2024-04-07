#pragma once

#include "Command.hpp"
#include "Defines.hpp"

namespace fro
{
	class GameObject;

	class GameObjectCommand : public Command
	{
		friend class InputManager;

	public:
		virtual ~GameObjectCommand() override = default;

	protected:
		GameObjectCommand(const GameObject& commandedGameObject);

		fro_NODISCARD_GETTER const GameObject& getCommandedGameObject() const;

	private:
		GameObjectCommand(const GameObjectCommand&) = delete;
		GameObjectCommand(GameObjectCommand&&) noexcept = delete;

		GameObjectCommand& operator=(const GameObjectCommand&) = delete;
		GameObjectCommand& operator=(GameObjectCommand&&) noexcept = delete;

		virtual void operator()() = 0;

		const GameObject& m_CommandedGameObject;
	};
}

#ifndef fro_GENERATED_GAME_OBJECT_COMMAND_BODY
#define fro_GENERATED_GAME_OBJECT_COMMAND_BODY(identifier)\
friend class InputManager;\
\
public:\
	virtual ~identifier() override; \
\
private:\
	identifier(const GameObject& commandedGameObject);\
	identifier(const identifier&) = delete;\
	identifier(identifier&&) noexcept = delete;\
\
	identifier& operator=(const identifier&) = delete;\
	identifier& operator=(identifier&&) noexcept = delete;\
\
	virtual void operator()() override;
#endif

#ifndef fro_GENERATED_GAME_OBJECT_COMMAND_CONSTRUCTOR
#define fro_GENERATED_GAME_OBJECT_COMMAND_CONSTRUCTOR(identifier)\
fro::identifier::identifier(const GameObject& commandedGameObject)\
	: GameObjectCommand(commandedGameObject)
#endif

#ifndef fro_GENERATED_GAME_OBJECT_COMMAND_DESTRUCTOR
#define fro_GENERATED_GAME_OBJECT_COMMAND_DESTRUCTOR(identifier)\
fro_GENERATED_COMMAND_DESTRUCTOR(identifier)
#endif

#ifndef fro_GENERATED_GAME_OBJECT_COMMAND_OPERATOR
#define fro_GENERATED_GAME_OBJECT_COMMAND_OPERATOR(identifier)\
fro_GENERATED_COMMAND_OPERATOR(identifier)
#endif