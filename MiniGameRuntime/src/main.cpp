#include <iostream>
#include "Math/Vec2.h"
#include "Components/TransformComponent.h"
#include "Components/VelocityComponent.h"
#include "Core/EntityManager.h"
#include "Core/World.h"

using namespace std;

int main() {
    cout << "MiniGameRuntime started." << endl;

    //调用带参构造函数创建a, b
    Vec2 a(1.0f, 2.0f);
    Vec2 b(3.0f, 4.0f);

    //调用带参构造函数创建c, 但是c又调用了成员方法
    Vec2 c = a + b;
    Vec2 d = a - b;
    Vec2 e = a * 2.5f;
    Vec2 g = a + b * 3.5f;

    cout << "c.x:" << c.x << " " << "c.y:" << c.y << endl;
    cout << "d.x:" << d.x << " " << "d.y:" << d.y << endl;
    cout << "e.x:" << e.x << " " << "e.y:" << e.y << endl;
    cout << "g.x:" << g.x << " " << "g.y:" << g.y << endl;

    //===========================================================
    //实现对象可以根据速度变更位置
    TransformComponent transform(Vec2(0.0f, 0.0f));
    VelocityComponent Velocity(Vec2(1.0f, 2.0f));

    float deltaTime = 0.016f;
    transform.position = transform.position + Velocity.velocity * deltaTime;

    cout << "X:" << transform.position.x << " " << "Y:" << transform.position.y << endl;

    //===========================================================
    cout << "===========================================================" << endl;
    //创建实体 
    EntityManager entityManager; //实例化我们的户籍管理局（EntityManager）

    Entity player = entityManager.CreateEntity(); //创建实体
    Entity enemy1 = entityManager.CreateEntity();
    Entity enemy2 = entityManager.CreateEntity();

    // 打印它们各自拿到的唯一 ID 编号
    cout << "Player Entity ID: " << player << endl;
    cout << "Enemy1 Entity ID: " << enemy1 << endl;
    cout << "Enemy2 Entity ID: " << enemy2 << endl;

    //输出当前存活的数量
    cout << "Living entity count =" << entityManager.GetEntityCount() << endl;

    //检查 enemy1 当前是否活着
    cout << "Enemy1 is alive? " << (entityManager.IsAlive(enemy1) ? "Yes" : "No") << endl;

    //干掉 enemy1
    cout << "Destroying Enemy1..." << endl;
    entityManager.DestroyEntity(enemy1);

    // 再次检查数量和存活状态
    cout << "Living entity count = " << entityManager.GetEntityCount() << endl;
    cout << "Enemy1 is alive now? " << (entityManager.IsAlive(enemy1) ? "Yes" : "No") << endl;
    cout << "Player is alive now? " << (entityManager.IsAlive(player) ? "Yes" : "No") << endl;

    //创建1000个Entity
    for (int i = 0; i < 1000; i++) {
        entityManager.CreateEntity();
    }

    cout << "一共有" << entityManager.GetEntityCount() << "个角色" << endl;

    //===========================================================
    cout << "===========================================================" << endl;
    cout << "--- 触发安全边界测试 ---" << endl;
    entityManager.DestroyEntity(INVALID_ENTITY); //删除无效实体
    entityManager.DestroyEntity(player); //正常删除
    entityManager.DestroyEntity(player); //重复删除
    entityManager.DestroyEntity(9999); //删除一个根本不存在的野数字

    //===========================================================
    cout << "===========================================================" << endl;
    cout << "--- World类测试 ---" << endl;
    World world;

    // 用world类创建实体
    Entity player2 = world.CreateEntity();
    cout << "Created Player Entity with ID: " << player << endl;

    // 初始位置定在 (10.0, 20.0)
    TransformComponent initialTransform(Vec2(10.0f, 20.0f));

    // 通过 World 将实体和位置组件关联
    world.AddTransform(player2, initialTransform);
    cout << "Successfully added TransformComponent to Player." << endl;

    // 现场创建 initialVelocity 结构体，并塞进速度账本
    VelocityComponent initialVelocity(Vec2(5.0f, -2.0f));
    world.AddVelocity(player2, initialVelocity);

    // 获取并打印当前位置
    TransformComponent& playerTransform = world.GetTransform(player2);
    cout << "Player current pos: (" << playerTransform.position.x
        << ", " << playerTransform.position.y << ")" << endl;

    // 获取并打印当前速度
    VelocityComponent& playerVelocity = world.GetVelocity(player2);
    cout << "Player current velocity: (" << playerVelocity.velocity.x
        << ", " << playerVelocity.velocity.y << ")" << endl;

    // 尝试在外部直接修改它的位置数据
    playerTransform.position.x = 55.5f;
    playerTransform.position.y = 88.8f;

    // 再次从世界中获取，验证账本里的是否也跟着变了
    TransformComponent& updatedTransform = world.GetTransform(player2);
    cout << "Player updated pos: (" << updatedTransform.position.x
        << ", " << updatedTransform.position.y << ")" << endl;

    //创建100个带 Transform 和 Velocity 的实体
    /*for (int i = 0; i < 100; i++) {
        Entity entity = world.CreateEntity();
        world.AddTransform(entity, TransformComponent(Vec2(float(i), 0.0f)));
        world.AddVelocity(entity, VelocityComponent(Vec2(1.0f, 2.0f)));
    }*/

    return 0;
}