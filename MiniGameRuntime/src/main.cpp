#include <iostream>

#include "Core/EntityManager.h"
#include "Components/TransformComponent.h"
#include "Components/VelocityComponent.h"
#include "Math/Vec2.h"
#include "Core/World.h"

int main()
{
    std::cout << "MiniGameRuntime Sandbox" << std::endl;

    World world;
    Entity player = world.CreateEntity(); // 创建实体，player保存的是实体ID

    //// 创建位置组件，初始位置为(2, 3)
    //Vec2 startingPosition{ 2.0f, 3.0f };
    //TransformComponent playerTransform{ startingPosition };

    //world.AddTransform(player, playerTransform);

    //Vec2 startingVelocity{ 4.0f, -2.0f };
    //VelocityComponent playerVelocity{ startingVelocity };

    //world.AddVelocity(player, playerVelocity);

    //bool hasTransform = world.HasTransform(player);
    //bool hasVelocity = world.HasVelocity(player);

    //std::cout << std::boolalpha;
    //std::cout << "Has transform: " << hasTransform << std::endl;
    //std::cout << "Has velocity: " << hasVelocity << std::endl;

    //TransformComponent& transform = world.GetTransform(player);
    //VelocityComponent& velocity = world.GetVelocity(player);

    // ===========================================================================
    std::cout << "=====================================" << std::endl;

    Entity player1 = world.CreateEntity();

    Vec2 startingPosition{ 2.0f, 3.0f };
    TransformComponent playerTransform{ startingPosition };
    world.AddTransform(player1, playerTransform);

    Vec2 startingVelocity{ 4.0f, -2.0f };
    VelocityComponent playerVelocity{ startingVelocity };
    world.AddVelocity(player1, playerVelocity);

    bool hasTransform1 = world.HasTransform(player1);
    bool hasVelocity1 = world.HasVelocity(player1);

    if (!hasTransform1 || !hasVelocity1) {
        std::cout << "Missing component!" << std::endl;
        return 1;
    }

    TransformComponent& initialTransform = world.GetTransform(player1);

    std::cout
        << "Initial position: ("
        << initialTransform.position.x << ","
        << initialTransform.position.y << ")"
        << std::endl;

    constexpr float deltaTime = 0.5f;

    for (int frame = 1; frame <= 3; frame++) {
        world.Update(deltaTime);

        TransformComponent& transform = world.GetTransform(player1);

        std::cout << "Frame" << frame << ":(" << transform.position.x << "," << transform.position.y << ")" << std::endl;
    }

    // ===========================================================================
    std::cout << "=====================================" << std::endl;

    Entity player2 = world.CreateEntity();
    Vec2 startingPosition{ 2.0f, 3.0f };
    TransformComponent playerTransform{ startingPosition };
    world.AddTransform(player2, playerTransform);

    Vec2 startingVelocity{ 4.0f, -2.0f };
    VelocityComponent playerVelocity{ startingVelocity };
    world.AddVelocity(player2, playerVelocity);

    constexpr float deltaTime = 0.5f;

    world.Update(deltaTime);

    TransformComponent& transform = world.GetTransform(player2);

    std::cout << "Position: (" << transform.position.x << ", " << transform.position.y << ")" << std::endl;

    // ===========================================================================
    std::cout << "=====================================" << std::endl;



    return 0;
}