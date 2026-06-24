#include <iostream>
#include "Math/Vec2.h"
#include "Components/TransformComponent.h"
#include "Components/VelocityComponent.h"
#include "Core/EntityManager.h"

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

    return 0;
}