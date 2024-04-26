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

		fro_NODISCARD GameObject const& getCommandedGameObject() const;

		virtual void operator()() = 0;

	protected:
		GameObjectCommand(GameObject const& commandedGameObject);

	private:
		GameObjectCommand(GameObjectCommand const&) = delete;
		GameObjectCommand(GameObjectCommand&&) noexcept = delete;

		GameObjectCommand& operator=(GameObjectCommand const&) = delete;
		GameObjectCommand& operator=(GameObjectCommand&&) noexcept = delete;

		GameObject const& m_CommandedGameObject;
	};
}