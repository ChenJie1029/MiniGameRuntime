#include <iostream>

#include "Core/EntityManager.h"

int main()
{
    std::cout << "MiniGameRuntime Sandbox" << std::endl;

    std::cout << std::boolalpha;

    EntityManager entityManager;

    Entity player = entityManager.CreateEntity();
    Entity enemy = entityManager.CreateEntity();

    bool firstDestroy = entityManager.DestroyEntity(enemy); // true

    bool secondDestroy = entityManager.DestroyEntity(enemy); // false

    bool invalidDestroy = entityManager.DestroyEntity(INVALID_ENTITY); // false

    std::cout << "First destroy: " << firstDestroy << std::endl; // true

    std::cout << "Second destroy: " << secondDestroy << std::endl; // false

    std::cout << "Invalid destroy: " << invalidDestroy << std::endl; // false

    std::cout << "Player alive: " << entityManager.IsAlive(player) << std::endl; // true

    std::cout << "Enemy alive: " << entityManager.IsAlive(enemy) << std::endl; // false

    std::cout << "Living count: " << entityManager.GetEntityCount() << std::endl; // 1

    return 0;
}