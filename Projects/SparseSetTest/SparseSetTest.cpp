#include "Components.hpp"
#include "ECS.h"
#include "ECSGroup.hpp"

#include <format>
#include <iostream>

int main()
{
	using namespace fro;

	ECS entityComponentSystem{};

	auto& tagTransformGroup{ entityComponentSystem.getGroup<TagComponent, TransfromComponent>() };
	auto& transformGroup{ entityComponentSystem.getGroup<TransfromComponent>() };
	auto& tagGroup{ entityComponentSystem.getGroup<TagComponent>() };

	GameObjectID gameObject0{ entityComponentSystem.createGameObject() };
	GameObjectID gameObject1{ entityComponentSystem.createGameObject() };
	GameObjectID gameObject2{ entityComponentSystem.createGameObject() };
	GameObjectID gameObject3{ entityComponentSystem.createGameObject() };
	GameObjectID gameObject4{ entityComponentSystem.createGameObject() };
	GameObjectID gameObject5{ entityComponentSystem.createGameObject() };
	GameObjectID gameObject6{ entityComponentSystem.createGameObject() };
	GameObjectID gameObject7{ entityComponentSystem.createGameObject() };
	GameObjectID gameObject8{ entityComponentSystem.createGameObject() };

	entityComponentSystem.addComponent<TagComponent>(gameObject0)->name = "Hi! I am 0";
	entityComponentSystem.addComponent<TagComponent>(gameObject1)->name = "Hi! I am 1";
	entityComponentSystem.addComponent<TagComponent>(gameObject2)->name = "Hi! I am 2";
	entityComponentSystem.addComponent<TransfromComponent>(gameObject0)->rotation = 0;
	entityComponentSystem.addComponent<TransfromComponent>(gameObject1)->rotation = 1;
	entityComponentSystem.addComponent<TransfromComponent>(gameObject2)->rotation = 2;
	entityComponentSystem.addComponent<TransfromComponent>(gameObject3)->rotation = 3;
	entityComponentSystem.addComponent<TransfromComponent>(gameObject4)->rotation = 4;
	entityComponentSystem.addComponent<TransfromComponent>(gameObject5)->rotation = 5;
	entityComponentSystem.addComponent<TagComponent>(gameObject6)->name = "Hi! I am 6";
	entityComponentSystem.addComponent<TagComponent>(gameObject7)->name = "Hi! I am 7";
	entityComponentSystem.addComponent<TagComponent>(gameObject8)->name = "Hi! I am 8";

	for (auto&& [gameObject, pTagComponent, pTransfromComponent] : tagTransformGroup)
		std::cout <<
		std::format("GameObjectID: {}\nTag: {}\nRotation: {}\n\n",
			gameObject,
			pTagComponent->name,
			pTransfromComponent->rotation);

	for (auto&& [gameObject, pTransfromComponent] : transformGroup)
		std::cout <<
		std::format("GameObjectID: {}\nRotation: {}\n\n",
			gameObject,
			pTransfromComponent->rotation);

	for (auto&& [gameObject, pTagComponent] : tagGroup)
		std::cout <<
		std::format("GameObjectID: {}\nTag: {}\n\n",
			gameObject,
			pTagComponent->name);
}