#pragma once

#include "Command.hpp"
#include "Defines.hpp"

namespace fro
{
	class GameObject;

	class GameObjectCommand : public Command
	{
	public:
		virtual ~GameObjectCommand() override = default;

		fro_NODISCARD const GameObject& getCommandedGameObject() const;

		virtual void operator()() = 0;

	protected:
		GameObjectCommand(const GameObject& commandedGameObject);

	private:
		GameObjectCommand(const GameObjectCommand&) = delete;
		GameObjectCommand(GameObjectCommand&&) noexcept = delete;

		GameObjectCommand& operator=(const GameObjectCommand&) = delete;
		GameObjectCommand& operator=(GameObjectCommand&&) noexcept = delete;

		const GameObject& m_CommandedGameObject;
	};
}