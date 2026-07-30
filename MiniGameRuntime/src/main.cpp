#include <iostream>
#include "Math/Vec2.h"
#include "Components/TransformComponent.h"
#include "Components/VelocityComponent.h"
#include "Core/EntityManager.h"
#include "Core/World.h"
#include "Physics/PhysicsUtils.h"
#include "Systems/CollisionSystem.h"
#include "Gameplay/CharacterStateMachine.h"
#include "Core/EventBus.h"
#include "Core/ObjectPool.h"
#include "Core/Timer.h"

// ===========================================================9 编写一个具体的业务对象：子弹
//class Bullet : public Hazel::IPoolable {
//public:
//    float x = 0.0f;
//    float y = 0.0f;
//    bool active = false;
//
//    // 唤醒逻辑
//    void Init() override {
//        x = 0.0f;
//        y = 0.0f;
//        active = true;
//        std::cout << "  [Bullet] 子弹激活：坐标归零 (" << x << ", " << y << ")\n";
//    }
//
//    // 归还逻辑
//    void Reset() override {
//        active = false;
//        std::cout << "  [Bullet] 子弹进入休眠状态\n";
//    }
//};

// ===========================================================10 Timer.h测试
class Bullet : public Hazel::IPoolable {
public:
    float x = 0, y = 0;
    void Init() override{}
    void Reset() override{}
};

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

    //===========================================================
    std::cout << "===========================================================" << std::endl;
    std::cout << "=== AABB Collision Real-Time Test ===" << std::endl;

    Entity player4 = world.CreateEntity();
    world.AddTransform(player4, TransformComponent(Vec2(0.0f, 0.0f)));
    world.AddAABB(player4, AABBComponent(32.0f, 32.0f));

    Entity enemy4 = world.CreateEntity();
    world.AddTransform(enemy4, TransformComponent(Vec2(100.0f, 100.0f)));
    world.AddAABB(enemy4, AABBComponent(32.0f, 32.0f));
    // ==========================================
    // 场景一测试：相隔很远
    // ==========================================
    std::cout << "\n--- Test 1: Entities are far apart ---" << std::endl;
    // 顺着网线取出各自的位置和碰撞大小
    TransformComponent& pTrans = world.GetTransform(player4);
    AABBComponent& pAABB = world.GetAABB(player4);

    TransformComponent& eTrans = world.GetTransform(enemy4);
    AABBComponent& eAABB = world.GetAABB(enemy4);

    bool isColliding1 = PhysicsUtils::CheckAABBCollision(
        pTrans.position, pAABB.halfExtents,
        eTrans.position, eAABB.halfExtents
    );

    std::cout << "Collision Status: " << (isColliding1 ? "💥 碰到!" : "🟢 没碰到") << std::endl;

    // 场景二测试：相隔很近
    // ==========================================
    std::cout << "\n--- Test 2: Entities are far apart ---" << std::endl;
    eTrans.position = Vec2(10.0f, 10.0f);
    bool isColliding2 = PhysicsUtils::CheckAABBCollision(
        pTrans.position, pAABB.halfExtents,
        eTrans.position, eAABB.halfExtents
    );
    std::cout << "Collision Status: " << (isColliding2 ? "💥 碰到!" : "🟢 没碰到") << std::endl;

    //===========================================================5
    std::cout << "===========================================================" << std::endl;
    std::cout << "=== CollisionSystem Automated Batch Test ===" << std::endl;

    Entity player5 = world.CreateEntity();
    world.AddTransform(player5, TransformComponent(Vec2(0.0f, 0.0f)));
    world.AddAABB(player5, AABBComponent(32.0f, 32.0f));

    Entity enemy5 = world.CreateEntity();
    world.AddTransform(enemy5, TransformComponent(Vec2(10.0f, 10.0f)));
    world.AddAABB(enemy5, AABBComponent(32.0f, 32.0f));

    Entity enemy6 = world.CreateEntity();
    world.AddTransform(enemy6, TransformComponent(Vec2(100.0f, 100.0f)));
    world.AddAABB(enemy6, AABBComponent(32.0f, 32.0f));

    Entity wall = world.CreateEntity();
    world.AddTransform(wall, TransformComponent(Vec2(0.0f, 20.0f)));
    world.AddAABB(wall, AABBComponent(64.0f, 16.0f));

    std::cout << "-> All 4 entities spawned and registered in World ledger." << std::endl;
    std::cout << "\n--- CollisionSystem Scanning Start ---" << std::endl;

    CollisionSystem collisionSystem;
    collisionSystem.Update(world);

    std::cout << "--- 🔍 Scan Finished ---" << std::endl;

    //===========================================================7
    std::cout << "===========================================================" << std::endl;
    std::cout << "=== Character State Machine Real-Time Test ===" << std::endl;

    Entity player7 = world.CreateEntity();
    std::cout << "-> Player Entity [" << player7 << "] created." << std::endl;

    // 实例化玩家的状态机大管家
    CharacterStateMachine playerFSM;

    playerFSM.ChangeState(player7, std::make_shared<IdleState>());

    // float deltaTime = 0.016f;
    for (int frame = 1; frame <= 3; frame++) {
        std::cout << "[Frame " << frame << "]" << std::endl;
        playerFSM.Update(player7, deltaTime);
    }

    // 让玩家再次切换状态（触发 Exit -> Enter 的完整交接）
    std::cout << "\n--- State Transition Test ---" << std::endl;
    // 为了看到效果，我们让他重新切一次 IdleState（或者下周切 MoveState）
    // 观察它会不会先触发老状态的 Exit，再触发新状态的 Enter
    playerFSM.ChangeState(player7, std::make_shared<IdleState>());

    std::cout << "\n=== Test Finished ===" << std::endl;

    //===========================================================8
    std::cout << "===========================================================" << std::endl;
    std::cout << "=== Engine Integration: EventBus & StateMachine Test ===" << std::endl;
    Entity player8 = world.CreateEntity();
    Entity monster8 = world.CreateEntity();
    std::cout << "-> Created Player8 [" << player8 << "] and Monster8 [" << monster8 << "].\n" << std::endl;

    // CharacterStateMachine playerFSM;  实例化玩家的状态机大管家

    // 让大管家去电报局登记自己的门牌号！
    EventBus::Subscribe(&playerFSM); // 因为大管家继承了 ICollisionListener，所以传入 &playerFSM（它的地址）是完全合法的
    std::cout << "[System] PlayerFSM subscribed to EventBus." << std::endl;

    // 开局让玩家进入 IdleState（站立发呆）
    std::cout << "\n--- Game Start: Setting Player to IDLE ---" << std::endl;
    playerFSM.ChangeState(player8, std::make_shared<IdleState>());

    // 模拟第一帧：玩家安全，一切平静
    std::cout << "\n--- Frame 1: Safe and Peaceful ---" << std::endl;
    playerFSM.Update(player8, 0.016f);

    // 模拟第二帧：物理系统突然检测到了碰撞！
    std::cout << "\n--- Frame 2: Danger! Collision Happens! ---" << std::endl;
    // CollisionSystem collisionSystem;  之前已经定义过了
    // 驱动物理系统。物理系统内部会检测到碰撞，现场写电报，并通过 EventBus.Publish 广播出去
    collisionSystem.Update(world);

    std::cout << "\n=== Test Finished ===" << std::endl;

    //===========================================================9
    std::cout << "===  Hazel 游戏引擎：对象池测试 ===\n\n";

    // 创建一个容量为 3 的子弹池（堆内存预分配 3 个 Bullet）
    Hazel::ObjectPool<Bullet> bulletPool(3);

    std::cout << "\n--- 1. 借出子弹 ---\n";
    Bullet* b1 = bulletPool.Acquire();
    Bullet* b2 = bulletPool.Acquire();

    std::cout << "\n--- 2. 修改子弹数据 ---\n";
    b1->x = 100.0f;
    b1->y = 200.0f;
    std::cout << "  b1 当前坐标: (" << b1->x << ", " << b1->y << ")\n";

    std::cout << "\n--- 3. 归还子弹b1 ---\n";
    bulletPool.Release(b1);

    std::cout << "\n--- 4. 再次借出子弹（验证内存复用与 Init 重置）---\n";
    Bullet* b3 = bulletPool.Acquire(); // b3 复用的就是刚刚归还的 b1 的内存
    std::cout << "  b3 坐标（应该被 Init 重置为 0）: (" << b3->x << ", " << b3->y << ")\n";

    std::cout << "\n=== 测试完成，准备退出 ===\n";

    //===========================================================10
    std::cout << "===  Hazel 游戏引擎：Timer测试 ===\n\n";
    constexpr int TEST_COUNT = 100000;

    // 1. 测试普通 new/delete 的耗时
    {
        Hazel::Timer timer;
        for (int i = 0; i < TEST_COUNT; ++i) {
            Bullet* b = new Bullet();
            delete b; // 每次都向操作系统申请和销毁内存
        }
        std::cout << "[原生 new/delete] 10 万次耗时: " << timer.ElapsedMilliseconds() << " ms\n";
    }

    // 2. 测试对象池 Acquire/Release 的耗时
    {
        Hazel::ObjectPool<Bullet> pool(TEST_COUNT);
        Hazel::Timer timer;
        for (int i = 0; i < TEST_COUNT; ++i) {
            Bullet* b = pool.Acquire();
            pool.Release(b);
        }
        std::cout << "[Hazel 对象池] 10 万次耗时: " << timer.ElapsedMilliseconds() << " ms\n";
    }

    return 0;
}