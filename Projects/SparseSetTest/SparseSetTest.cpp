#include "Components.hpp"
#include "ECS.h"

#include <iostream>

int main()
{
	using namespace fro;

	fro::ECS entityComponentSystem{};

	entityComponentSystem.addComponent<TagComponent>(0)->name = "Hey!";
	entityComponentSystem.addComponent<TagComponent>(2)->name = "Hey! I am 2!";
	std::cout << entityComponentSystem.getComponent<TagComponent>(2)->name << std::endl;
	entityComponentSystem.removeComponent<TagComponent>(0);
	std::cout << entityComponentSystem.getComponent<TagComponent>(2)->name << " ... still :)" << std::endl;
}