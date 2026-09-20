#include <iostream>

#include "Core/World.h"
#include "Systems/CollisionSystem.h"

bool HandleInput(World& world, Entity player) {
    while (true) {
        std::cout << "Move (W/A/S/D, Q to quit): ";

        char input;

        if (!(std::cin >> input)) {
            std::cout << "Input error!" << std::endl;
            return false;
        }

        VelocityComponent& velocity = world.GetVelocity(player);

        switch (input) {
        case 'W':
        case 'w':
            velocity.velocity = Vec2{ 0.0f, 2.0f };
            return true;
        case 'A':
        case 'a':
            velocity.velocity = Vec2{ -2.0f, 0.0f };
            return true;
        case 'S':
        case 's':
            velocity.velocity = Vec2{0.0f, -2.0f};
            return true;
        case 'D':
        case 'd':
            velocity.velocity = Vec2{ 2.0f, 0.0f };
            return true;
        case 'Q':
        case 'q':
            return false;
        default:
            std::cout << "Re-enter!" << std::endl;
            continue;
        }
    }
}

void PrintPosition(World& world, Entity entity) {
    const TransformComponent& transform = world.GetTransform(entity);

    std::cout
        << "Player position: ("
        << transform.position.x << ", "
        << transform.position.y << ")"
        << std::endl;
}

void RunSandbox() {
    World world;
    CollisionSystem collisionSystem;

    Entity boxA = world.CreateEntity();
    world.AddTransform(boxA, TransformComponent{ Vec2{0.0f, 0.0f} });
    world.AddVelocity(boxA, VelocityComponent{ Vec2{0.0f, 0.0f} });
    world.AddAABB(boxA, AABBComponent{ 1.0f, 1.0f });

    Entity boxB = world.CreateEntity();
    world.AddTransform(boxB, TransformComponent{ Vec2{5.0f, 0.0f} });
    world.AddAABB(boxB, AABBComponent{ 1.0f, 1.0f });

    constexpr float deltaTime = 0.5f;

    while (HandleInput(world, boxA)) {
        world.Update(deltaTime);
        collisionSystem.Update(world);
        PrintPosition(world, boxA);
    }

    std::cout << "Sandbox closed." << std::endl;
}

int main()
{
    std::cout << "MiniGameRuntime Sandbox" << std::endl;

    RunSandbox();

    return 0;
}