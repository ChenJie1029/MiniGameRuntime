# Stage4_Code_History

~~~c++
#include <iostream>

//#include "Core/EntityManager.h"
//#include "Components/TransformComponent.h"
//#include "Components/VelocityComponent.h"
//#include "Math/Vec2.h"
//#include "Physics/PhysicsUtils.h"
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

//void TestEntityDestruction() {
//    World testWorld;
//
//    Entity entity = testWorld.CreateEntity();
//
//    testWorld.AddTransform(entity, TransformComponent{ Vec2{1.0f, 2.0f} });
//    testWorld.AddVelocity(entity, VelocityComponent{ Vec2{3.0f, 4.0f} });
//    testWorld.AddAABB(entity, AABBComponent{ 1.0f, 1.0f });
//
//    std::cout << std::boolalpha;
//
//    // 输出销毁前的三项检查
//    std::cout << "Before destroy:" << std::endl;
//    std::cout
//        << "Transform: " << testWorld.HasTransform(entity)
//        << std::endl;
//    std::cout
//        << "Velocity: " << testWorld.HasVelocity(entity)
//        << std::endl;
//    std::cout
//        << "AABB: " << testWorld.HasAABB(entity)
//        << std::endl;
//
//    testWorld.DestroyEntity(entity);
//
//    // 输出销毁后的三项检查
//    std::cout << "After destroy:" << std::endl;
//    std::cout
//        << "Transform: " << testWorld.HasTransform(entity)
//        << std::endl;
//    std::cout
//        << "Velocity: " << testWorld.HasVelocity(entity)
//        << std::endl;
//    std::cout
//        << "AABB: " << testWorld.HasAABB(entity)
//        << std::endl;
//
//    // 重复销毁测试
//    testWorld.DestroyEntity(entity);
//}

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
    //std::cout << "MiniGameRuntime Sandbox" << std::endl;

    //World world;
    //Entity player = world.CreateEntity(); // 创建实体，player保存的是实体ID

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
    //std::cout << "=====================================" << std::endl;

    //Entity player1 = world.CreateEntity();

    //Vec2 startingPosition{ 2.0f, 3.0f };
    //TransformComponent playerTransform{ startingPosition };
    //world.AddTransform(player1, playerTransform);

    //Vec2 startingVelocity{ 4.0f, -2.0f };
    //VelocityComponent playerVelocity{ startingVelocity };
    //world.AddVelocity(player1, playerVelocity);

    //bool hasTransform1 = world.HasTransform(player1);
    //bool hasVelocity1 = world.HasVelocity(player1);

    //if (!hasTransform1 || !hasVelocity1) {
    //    std::cout << "Missing component!" << std::endl;
    //    return 1;
    //}

    //TransformComponent& initialTransform = world.GetTransform(player1);

    //std::cout
    //    << "Initial position: ("
    //    << initialTransform.position.x << ","
    //    << initialTransform.position.y << ")"
    //    << std::endl;

    //constexpr float deltaTime = 0.5f;

    //for (int frame = 1; frame <= 3; frame++) {
    //    world.Update(deltaTime);

    //    TransformComponent& transform = world.GetTransform(player1);

    //    std::cout << "Frame" << frame << ":(" << transform.position.x << "," << transform.position.y << ")" << std::endl;
    //}

    //// ===========================================================================
    //std::cout << "=====================================" << std::endl;

    //Entity player2 = world.CreateEntity();
    //Vec2 startingPosition{ 2.0f, 3.0f };
    //TransformComponent playerTransform{ startingPosition };
    //world.AddTransform(player2, playerTransform);

    //Vec2 startingVelocity{ 4.0f, -2.0f };
    //VelocityComponent playerVelocity{ startingVelocity };
    //world.AddVelocity(player2, playerVelocity);

    //constexpr float deltaTime = 0.5f;

    //world.Update(deltaTime);

    //TransformComponent& transform = world.GetTransform(player2);

    //std::cout << "Position: (" << transform.position.x << ", " << transform.position.y << ")" << std::endl;

    // ===========================================================================
    //std::cout << "=====================================" << std::endl;

    //World world;
    //Entity player1 = world.CreateEntity();
    //Vec2 player1startingPosition{ 1.0f, 2.0f };
    //TransformComponent playerTransform{ player1startingPosition };
    //world.AddTransform(player1, playerTransform);
    //Vec2 startingVelocity{ 3.0f, -1.0f };
    //VelocityComponent playerVelocity{ startingVelocity };
    //world.AddVelocity(player1, playerVelocity);

    //Entity wall = world.CreateEntity();
    //Vec2 wallstartingPosition{ 10.0f, 5.0f };
    //TransformComponent wallTransform{ wallstartingPosition };
    //world.AddTransform(wall, wallTransform);

    //constexpr float deltaTime = 0.5f;

    //for (int frame = 1; frame <= 2; frame++) {
    //    world.Update(deltaTime);

    //    TransformComponent& player1Transform = world.GetTransform(player1);
    //    TransformComponent& wallTransform = world.GetTransform(wall);

    //    std::cout << "Position:(" << player1Transform.position.x << ", " << player1Transform.position.y << ")" << std::endl;
    //    std::cout << "Position:(" << wallTransform.position.x << ", " << wallTransform.position.y << ")" << std::endl;
    //}

    // ===========================================================================
    //std::cout << "=====================================" << std::endl;

    //World world;

    //Entity boxA = world.CreateEntity();
    //world.AddTransform(boxA, TransformComponent{ Vec2{0.0f, 0.0f} });
    //world.AddAABB(boxA, AABBComponent{ 4.0f, 4.0f });

    //Entity boxB = world.CreateEntity();
    //world.AddTransform(boxB, TransformComponent{ Vec2{5.0f, 0.0f} });
    //world.AddAABB(boxB, AABBComponent{ 2.0f, 2.0f });

    //TransformComponent& transformA = world.GetTransform(boxA);
    //AABBComponent& aabbA = world.GetAABB(boxA);

    //TransformComponent& transformB = world.GetTransform(boxB);
    //AABBComponent& aabbB = world.GetAABB(boxB);

    //bool beforeMove = PhysicsUtils::CheckAABBCollision(
    //    transformA.position,
    //    aabbA.halfExtents,
    //    transformB.position,
    //    aabbB.halfExtents
    //);

    //transformB.position.x = 3.0f;
    //transformB.position.y = 0.0f;

    //bool afterMove = PhysicsUtils::CheckAABBCollision(
    //    transformA.position,
    //    aabbA.halfExtents,
    //    transformB.position,
    //    aabbB.halfExtents
    //);

    //std::cout << std::boolalpha;
    //std::cout << "Before move: " << beforeMove << std::endl;
    //std::cout << "After move: " << afterMove << std::endl;

    // ===========================================================================
    //std::cout << "==================Day 20===================" << std::endl;

    //World world;

    //CollisionSystem collisionSystem;

    //Entity boxA = world.CreateEntity();
    //world.AddTransform(boxA, TransformComponent{ Vec2{0.0f, 0.0f} });
    //world.AddAABB(boxA, AABBComponent{ 1.0f, 1.0f });

    //Entity boxB = world.CreateEntity();
    //world.AddTransform(boxB, TransformComponent{ Vec2{5.0f, 0.0f} });
    //world.AddAABB(boxB, AABBComponent{ 1.0f, 1.0f });

    //std::cout << "First check:" << std::endl;
    //collisionSystem.Update(world);

    //TransformComponent& transformB = world.GetTransform(boxB);
    //transformB.position.x = 1.0f;

    //std::cout << "Second check:";
    //collisionSystem.Update(world);

    // ===========================================================================
    //std::cout << "==================Day 21===================" << std::endl;

    //World world;

    //CollisionSystem collisionSystem;

    //Entity boxA = world.CreateEntity();
    //world.AddTransform(boxA, TransformComponent{ Vec2{0.0f, 0.0f} });
    //world.AddVelocity(boxA, VelocityComponent{ Vec2{0.0f, 0.0f} });
    //world.AddAABB(boxA, AABBComponent{ 1.0f, 1.0f });

    //Entity boxB = world.CreateEntity();
    //world.AddTransform(boxB, TransformComponent{ Vec2{5.0f, 0.0f} });
    //world.AddAABB(boxB, AABBComponent{ 1.0f, 1.0f });

    //VelocityComponent& velocity = world.GetVelocity(boxA);

    //constexpr float deltaTime = 0.5f;

    //bool quit = false;
    //while (!quit) {
    //    std::cout << "Move (A/D, Q to quit): ";
    //    char input;
    //    
    //    if (!(std::cin >> input)) {
    //        std::cout << "Input error!" << std::endl;
    //        break;
    //    }

    //    switch (input) {
    //    case 'D':
    //    case 'd':
    //        velocity.velocity.x = 2.0f;
    //        break;
    //    case 'A':
    //    case 'a':
    //        velocity.velocity.x = -2.0f;
    //        break;
    //    case 'Q':
    //    case 'q':
    //        quit = true;
    //        break;
    //    default:
    //        std::cout << "Re-enter!" << std::endl;
    //        continue;
    //    }

    //    if (quit) {
    //        break;
    //    }

    //    world.Update(deltaTime);
    //    collisionSystem.Update(world);

    //    const TransformComponent& transform = world.GetTransform(boxA);

    //    std::cout
    //        << "Player position: ("
    //        << transform.position.x << ", "
    //        << transform.position.y << ")"
    //        << std::endl;

    //}

    // ===========================================================================
    //std::cout << "==================Day 22===================" << std::endl;

    //World world;

    //Entity boxA = world.CreateEntity();
    //world.AddTransform(boxA, TransformComponent{ Vec2{0.0f, 0.0f} });
    //world.AddVelocity(boxA, VelocityComponent{ Vec2{0.0f, 0.0f} });
    //world.AddAABB(boxA, AABBComponent{ 1.0f, 1.0f });

    //Entity boxB = world.CreateEntity();
    //world.AddTransform(boxB, TransformComponent{ Vec2{5.0f, 0.0f} });
    //world.AddAABB(boxB, AABBComponent{ 1.0f, 1.0f });

    //CollisionSystem collisionSystem;
    //constexpr float deltaTime = 0.5f;

    //while (HandleInput(world, boxA)) {
    //    world.Update(deltaTime);
    //    collisionSystem.Update(world);
    //    PrintPosition(world, boxA);
    //}

    //std::cout << "==================Day 23===================" << std::endl;
    std::cout << "MiniGameRuntime Sandbox" << std::endl;

    //TestEntityDestruction();

    RunSandbox();

    return 0;
}
~~~

