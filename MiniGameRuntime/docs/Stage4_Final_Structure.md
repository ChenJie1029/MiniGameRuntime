# Stage4_Final_Structure

## 当前入口流程

当前真实启动流程：

~~~c++
main()
→ RunSandbox()
→ 创建World和CollisionSystem
→ 创建玩家与障碍物
→ 添加组件
→ 进入循环
~~~

一轮循环：

~~~c++
HandleInput()
→ 设置玩家速度
→ World::Update(deltaTime)
→ 更新玩家位置
→ CollisionSystem::Update(world)
→ 筛选同时拥有Transform和AABB的实体
→ 实体两两配对
→ PhysicsUtils::CheckAABBCollision()
→ 碰撞时输出日志并发布CollisionEvent
→ PrintPosition()
→ 返回下一轮输入
~~~

## 文件职责

| 文件                     | 需要说明的内容                                               |
| ------------------------ | ------------------------------------------------------------ |
| `main.cpp`               | 程序入口、场景初始化和循环组织                               |
| `Entity.h`               | `Entity` 实际std::uint32_t，`Entity` 只是这个整数类型的别名，无效ID是0 |
| `EntityManager.h/.cpp`   | EntityManager负责分配Entity ID、保存当前存活ID，并删除指定ID。<br/>World::DestroyEntity()先通知EntityManager删除ID，再从三个组件容器中删除对应组件 |
| `World.h/.cpp`           | World内部持有EntityManager和三个组件容器，<br/>提供组件添加、查询和存在性检查接口，<br/>Update()根据速度更新位置，<br/>DestroyEntity()同步清理实体及其组件 |
| `TransformComponent.h`   | 实体的位置                                                   |
| `VelocityComponent.h`    | 实体的速度                                                   |
| `AABBComponent.h`        | 实体的碰撞范围                                               |
| `CollisionSystem.h/.cpp` | 双层for循环进行配对                                          |
| `PhysicsUtils.h`         | 负责计算碰撞边界                                             |
| `EventBus.h/.cpp`        | 接收发布的CollisionEvent，并通知已经注册的碰撞监听者         |

## 数据归属

> Entity ID由EntityManager创建和管理。
>
> TransformComponent、VelocityComponent和AABBComponent
> 定义组件的数据结构。
>
> 具体的组件对象保存在World的三个unordered_map中，
> 并通过Entity ID作为键进行关联。
>
> CollisionSystem不拥有组件，
> 它从World读取组件，并临时创建targets名单进行碰撞检查。
>
> main.cpp中的RunSandbox创建局部World，
> 实际组件容器属于这个World对象。

> Component.h：定义数据长什么样 
>
> World中的unordered_map：真正保存组件数据

## 三个 Update

| 调用                        | 作用                 |
| --------------------------- | -------------------- |
| `HandleInput()`             | 设置本轮移动意图     |
| `World::Update()`           | 根据速度更新位置     |
| `CollisionSystem::Update()` | 检查当前位置是否碰撞 |

## 执行顺序原因

> HandleInput先设置本轮速度；
> World::Update根据速度计算新位置；
> CollisionSystem随后检查更新后的新位置；
> PrintPosition最后显示这一帧的结果。
> 如果先检查碰撞再移动，检查到的就是上一帧的位置。