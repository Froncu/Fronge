#include "Components.hpp"
#include "ECS.h"
#include "ECSGroup.hpp"

#include <iostream>

int main()
{
	using namespace fro;

	fro::ECS entityComponentSystem{};

	auto& group = entityComponentSystem.getGroup<TagComponent, TransfromComponent>();

	GameObjectID gameObject0{ entityComponentSystem.createGameObject() };
	GameObjectID gameObject1{ entityComponentSystem.createGameObject() };
	GameObjectID gameObject2{ entityComponentSystem.createGameObject() };

	entityComponentSystem.addComponent<TagComponent>(gameObject0)->name = "Hi! I am 1";
	entityComponentSystem.addComponent<TagComponent>(gameObject1)-> name = "Hi! I am 2";
	entityComponentSystem.addComponent<TagComponent>(gameObject2)-> name = "Hi! I am 3";
	entityComponentSystem.addComponent<TransfromComponent>(gameObject0);
	entityComponentSystem.removeComponent<TransfromComponent>(gameObject0);
	entityComponentSystem.addComponent<TransfromComponent>(gameObject2);

	for (auto&& [gameObject, pTagComponent, pTransfromComponent] : group)
	{
		std::cout << gameObject << " -> " << pTagComponent->name << std::endl;
	}


	std::reference_wrapper a{ *entityComponentSystem.getComponent<TransfromComponent>(1) };
}