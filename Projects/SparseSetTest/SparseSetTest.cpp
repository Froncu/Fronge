#include "Components.hpp"
#include "GameObject.h"
#include "SparseSet.hpp"

#include <format>
#include <iostream>

fro::GameObject gameObject1{};
fro::GameObject gameObject2{};
fro::GameObject gameObject3{};
fro::SparseSet<fro::TagComponent> ssTagComponents{};

void doTests()
{
    std::cout
        << "contains GameObject1 -> " << ssTagComponents.contains(gameObject1) << '\n'
        << "contains GameObject2 -> " << ssTagComponents.contains(gameObject2) << '\n'
        << "contains GameObject3 -> " << ssTagComponents.contains(gameObject3) << "\n\n"

        << "find GameObject1 -> " << ssTagComponents.find(gameObject1) << '\n'
        << "find GameObject2 -> " << ssTagComponents.find(gameObject2) << '\n'
        << "find GameObject3 -> " << ssTagComponents.find(gameObject3) << "\n\n"

        << "remove GameObject1 -> " << ssTagComponents.remove(gameObject1) << '\n'
        << "remove GameObject2 -> " << ssTagComponents.remove(gameObject2) << '\n'
        << "remove GameObject3 -> " << ssTagComponents.remove(gameObject3) << "\n\n"

        << "insert GameObject1 -> " << ssTagComponents.insert(gameObject1) << '\n'
        << "insert GameObject2 -> " << ssTagComponents.insert(gameObject2) << '\n'
        << "insert GameObject3 -> " << ssTagComponents.insert(gameObject3) << "\n\n"

        << "contains GameObject1 -> " << ssTagComponents.contains(gameObject1) << '\n'
        << "contains GameObject2 -> " << ssTagComponents.contains(gameObject2) << '\n'
        << "contains GameObject3 -> " << ssTagComponents.contains(gameObject3) << "\n\n"

        << "find GameObject1 -> " << ssTagComponents.find(gameObject1) << '\n'
        << "find GameObject2 -> " << ssTagComponents.find(gameObject2) << '\n'
        << "find GameObject3 -> " << ssTagComponents.find(gameObject3) << "\n\n"

        << "remove GameObject1 -> " << ssTagComponents.remove(gameObject1) << '\n'
        << "remove GameObject2 -> " << ssTagComponents.remove(gameObject2) << '\n'
        << "remove GameObject3 -> " << ssTagComponents.remove(gameObject3) << "\n\n";

    ssTagComponents.clear();
}

int main()
{
    std::cout << "TEST 1: no inserted GameObjects\n";
    doTests();

    std::cout << "TEST 2: inserted GameObject2\n";
    ssTagComponents.insert(gameObject2);
    doTests();

    std::cout << "TEST 3: inserted GameObject2 & GameObject3\n";
    ssTagComponents.insert(gameObject2);
    ssTagComponents.insert(gameObject3);
    doTests();

    std::cout << "TEST 4: inserted all GameObjects\n";
    ssTagComponents.insert(gameObject1);
    ssTagComponents.insert(gameObject2);
    ssTagComponents.insert(gameObject3);
    doTests();
}