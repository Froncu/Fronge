#include "Components.hpp"
#include "ECS.h"
#include "ECSGroup.hpp"

#include <iostream>

int main()
{
	using namespace fro;

	fro::ECS entityComponentSystem{};
	GameObjectID gameObject0{ entityComponentSystem.createGameObject() };
	GameObjectID gameObject1{ entityComponentSystem.createGameObject() };
	GameObjectID gameObject2{ entityComponentSystem.createGameObject() };
	entityComponentSystem.createGameObject();
	entityComponentSystem.destroyGameObject(gameObject2);
	gameObject2 = entityComponentSystem.createGameObject();

	entityComponentSystem.addComponent<TagComponent>(gameObject0)->name = "Hey!";
	entityComponentSystem.addComponent<TagComponent>(gameObject1)->name = "Hey! I am 2!";
	std::cout << entityComponentSystem.getComponent<TagComponent>(gameObject1)->name << std::endl;
	entityComponentSystem.removeComponent<TagComponent>(gameObject0);
	std::cout << entityComponentSystem.getComponent<TagComponent>(gameObject1)->name << " ... still :)\n\n";

	entityComponentSystem.getGroup<TagComponent, TransfromComponent>();
}