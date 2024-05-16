#include "Components.hpp"
#include "ComponentSet.hpp"
#include "GameObject.h"

#include <iostream>

int main()
{
	using namespace fro;

	std::vector<GameObject> vGameObjects(10);
	ComponentSet<TagComponent> csTagComponents{};

	csTagComponents.insert(vGameObjects[0])->name = "GameObject 1 :)";
	csTagComponents.insert(vGameObjects[5])->name = "GameObject 5 :D";
	csTagComponents.insert(vGameObjects[4])->name = "GameObject 4 B)";
	csTagComponents.insert(vGameObjects[7])->name = "GameObject 7 o7";
	csTagComponents.insert(vGameObjects[9])->name = "GameObject 9 >_<";

	csTagComponents.swap(vGameObjects[9], 1);

	std::cout
		<< csTagComponents.find(vGameObjects[9])->name
		<< std::endl
		<< csTagComponents.find(vGameObjects[5])->name;
}