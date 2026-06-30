#include <iostream>
#include "Math/Vec2.h"
#include "Components/TransformComponent.h"
#include "Components/VelocityComponent.h"
#include "Core/EntityManager.h"
#include "Core/World.h"

int main() {
    std::cout << "MiniGameRuntime started." << std::endl;

    //调用带参构造函数创建a, b
    Vec2 a(1.0f, 2.0f);
    Vec2 b(3.0f, 4.0f);

    //调用带参构造函数创建c, 但是c又调用了成员方法
    Vec2 c = a + b;
    Vec2 d = a - b;
    Vec2 e = a * 2.5f;
    Vec2 g = a + b * 3.5f;

    std::cout << "c.x:" << c.x << " " << "c.y:" << c.y << std::endl;
    std::cout << "d.x:" << d.x << " " << "d.y:" << d.y << std::endl;
    std::cout << "e.x:" << e.x << " " << "e.y:" << e.y << std::endl;
    std::cout << "g.x:" << g.x << " " << "g.y:" << g.y << std::endl;

    //===========================================================
    //实现对象可以根据速度变更位置
    TransformComponent transform(Vec2(0.0f, 0.0f));
    VelocityComponent Velocity(Vec2(1.0f, 2.0f));

    float deltaTime = 0.016f;
    transform.position = transform.position + Velocity.velocity * deltaTime;

    std::cout << "X:" << transform.position.x << " " << "Y:" << transform.position.y << std::endl;

    //===========================================================
    std::cout << "===========================================================" << std::endl;
    //创建实体 
    EntityManager entityManager; //实例化我们的户籍管理局（EntityManager）

    Entity player = entityManager.CreateEntity(); //创建实体
    Entity enemy1 = entityManager.CreateEntity();
    Entity enemy2 = entityManager.CreateEntity();

    // 打印它们各自拿到的唯一 ID 编号
    std::cout << "Player Entity ID: " << player << std::endl;
    std::cout << "Enemy1 Entity ID: " << enemy1 << std::endl;
    std::cout << "Enemy2 Entity ID: " << enemy2 << std::endl;

    //输出当前存活的数量
    std::cout << "Living entity count =" << entityManager.GetEntityCount() << std::endl;

    //检查 enemy1 当前是否活着
    std::cout << "Enemy1 is alive? " << (entityManager.IsAlive(enemy1) ? "Yes" : "No") << std::endl;

    //干掉 enemy1
    std::cout << "Destroying Enemy1..." << std::endl;
    entityManager.DestroyEntity(enemy1);

    // 再次检查数量和存活状态
    std::cout << "Living entity count = " << entityManager.GetEntityCount() << std::endl;
    std::cout << "Enemy1 is alive now? " << (entityManager.IsAlive(enemy1) ? "Yes" : "No") << std::endl;
    std::cout << "Player is alive now? " << (entityManager.IsAlive(player) ? "Yes" : "No") << std::endl;

    //创建1000个Entity
    for (int i = 0; i < 1000; i++) {
        entityManager.CreateEntity();
    }

    std::cout << "一共有" << entityManager.GetEntityCount() << "个角色" << std::endl;

    //===========================================================
    std::cout << "===========================================================" << std::endl;
    std::cout << "--- 触发安全边界测试 ---" << std::endl;
    entityManager.DestroyEntity(INVALID_ENTITY); //删除无效实体
    entityManager.DestroyEntity(player); //正常删除
    entityManager.DestroyEntity(player); //重复删除
    entityManager.DestroyEntity(9999); //删除一个根本不存在的野数字

    //===========================================================
    std::cout << "===========================================================" << std::endl;
    std::cout << "--- World类测试 ---" << std::endl;
    World world;

    // 用world类创建实体
    Entity player2 = world.CreateEntity();
    std::cout << "Created Player Entity with ID: " << player << std::endl;

    // 初始位置定在 (10.0, 20.0)
    TransformComponent initialTransform(Vec2(10.0f, 20.0f));

    // 通过 World 将实体和位置组件关联
    world.AddTransform(player2, initialTransform);
    std::cout << "Successfully added TransformComponent to Player." << std::endl;

    // 现场创建 initialVelocity 结构体，并塞进速度账本
    VelocityComponent initialVelocity(Vec2(5.0f, -2.0f));
    world.AddVelocity(player2, initialVelocity);

    // 获取并打印当前位置
    TransformComponent& playerTransform = world.GetTransform(player2);
    std::cout << "Player current pos: (" << playerTransform.position.x
        << ", " << playerTransform.position.y << ")" << std::endl;

    // 获取并打印当前速度
    VelocityComponent& playerVelocity = world.GetVelocity(player2);
    std::cout << "Player current velocity: (" << playerVelocity.velocity.x
        << ", " << playerVelocity.velocity.y << ")" << std::endl;

    // 尝试在外部直接修改它的位置数据
    playerTransform.position.x = 55.5f;
    playerTransform.position.y = 88.8f;

    // 再次从世界中获取，验证账本里的是否也跟着变了
    TransformComponent& updatedTransform = world.GetTransform(player2);
    std::cout << "Player updated pos: (" << updatedTransform.position.x
        << ", " << updatedTransform.position.y << ")" << std::endl;

    //创建100个带 Transform 和 Velocity 的实体
    /*for (int i = 0; i < 100; i++) {
        Entity entity = world.CreateEntity();
        world.AddTransform(entity, TransformComponent(Vec2(float(i), 0.0f)));
        world.AddVelocity(entity, VelocityComponent(Vec2(1.0f, 2.0f)));
    }*/

    //===========================================================
    std::cout << "===========================================================" << std::endl;
    std::cout << "--- Game Loop & World Update Simulation ---" << std::endl;
    Entity player3 = world.CreateEntity();
    world.AddTransform(player3, TransformComponent(Vec2(0.0f, 0.0f)));
    world.AddVelocity(player3, VelocityComponent(Vec2(10.0f, 5.0f)));

    // 模拟游戏主循环(Game Loop)，跑 5 帧
    // float deltaTime = 0.016f;真实的值
    deltaTime = 1.0f; // 假设每帧雷打不动过去 1 秒

    for (int frame = 1; frame <= 5; frame++) {
        world.Update(deltaTime);

        TransformComponent& pTrans = world.GetTransform(player3);
        std::cout << "[Frame " << frame << "] Player3 Pos: ("
            << pTrans.position.x << ", " << pTrans.position.y << ")" << std::endl;
    }

    return 0;
}