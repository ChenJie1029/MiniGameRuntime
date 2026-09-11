#include <iostream>

#include "Core/EntityManager.h"

int main()
{
    std::cout << "MiniGameRuntime Sandbox" << std::endl;

    EntityManager entityManager;

    Entity player = entityManager.CreateEntity();
    Entity enemy = entityManager.CreateEntity();

    std::cout << "Player ID: " << player << std::endl;

    std::cout << "Enemy ID: " << enemy << std::endl;

    std::cout << "Living count: " << entityManager.GetEntityCount() << std::endl;

    entityManager.DestroyEntity(enemy);

    std::cout << "Enemy alive: " << entityManager.IsAlive(enemy) << std::endl;

    std::cout << "Living count: " << entityManager.GetEntityCount() << std::endl;

    return 0;
}