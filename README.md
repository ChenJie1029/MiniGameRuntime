# MiniGameRuntime

MiniGameRuntime 是一个使用现代 C++ 编写的控制台游戏运行时练习项目。项目以“实体 ID + 独立组件容器”的方式组织数据，用较小的代码规模演示实体管理、组件管理、逐帧移动、AABB 碰撞检测以及交互式 Sandbox 主循环。

本项目的重点不是制作完整游戏，而是理解一个小型游戏运行时如何划分数据、逻辑和系统职责，并为后续学习 SDL2、OpenGL、ECS 和更完整的游戏引擎结构打下基础。

## 当前功能

- 创建唯一的 `Entity` ID；
- 查询实体是否存活以及当前存活实体数量；
- 销毁实体，并清理该实体拥有的组件；
- 为实体添加和查询位置、速度及 AABB 碰撞组件；
- 使用速度和 `deltaTime` 更新实体位置；
- 筛选具备位置与碰撞组件的实体；
- 对候选实体进行两两 AABB 碰撞检测；
- 在控制台中使用 `W/A/S/D` 控制玩家移动；
- 输入 `Q` 退出 Sandbox；
- 对输入失败、缺少组件和重复销毁等情况进行基本保护；
- 通过结构说明和测试报告记录阶段四的设计与验收结果。

## 操作方式

| 输入 | 功能 |
|---|---|
| `W` / `w` | 向上移动 |
| `A` / `a` | 向左移动 |
| `S` / `s` | 向下移动 |
| `D` / `d` | 向右移动 |
| `Q` / `q` | 退出 Sandbox |

每次有效移动输入代表一帧：程序先设置玩家速度，再更新世界、执行碰撞检测，最后输出玩家的新位置。

## 核心结构

| 模块 | 主要职责 |
|---|---|
| `Entity` | 实体的唯一数字 ID，不直接保存位置、速度等游戏数据 |
| `EntityManager` | 创建、查询和销毁实体，并维护当前存活实体列表 |
| `TransformComponent` | 保存实体的位置数据 |
| `VelocityComponent` | 保存实体的速度数据 |
| `AABBComponent` | 保存轴对齐包围盒的半尺寸数据 |
| `Vec2` | 表示二维向量，用于位置和速度计算 |
| `World` | 集中管理实体及组件容器，并根据速度更新位置 |
| `PhysicsUtils` | 提供不依赖具体对象的 AABB 碰撞计算 |
| `CollisionSystem` | 筛选可碰撞实体、生成实体组合并调用碰撞检测 |
| `CollisionEvent` / `EventBus` | 表示和分发碰撞事件，为系统之间的解耦提供基础 |
| `main.cpp` | 创建 Sandbox 场景、处理输入并组织主循环 |

## 数据组织方式

`Entity` 的真实类型是整数 ID。位置、速度和碰撞数据分别保存在 `World` 内部的组件容器中，并以实体 ID 作为键。

```cpp
Entity player = world.CreateEntity();

world.AddTransform(player, TransformComponent{Vec2{0.0f, 0.0f}});
world.AddVelocity(player, VelocityComponent{Vec2{0.0f, 0.0f}});
world.AddAABB(player, AABBComponent{1.0f, 1.0f});
```

这种结构使实体身份与实体数据相互分离。同一个实体可以拥有不同组件，而系统只处理自己需要的组件。

## 一帧的执行流程

1. `HandleInput()` 读取 `W/A/S/D` 或 `Q`；
2. 根据输入修改玩家的速度组件；
3. `World::Update(deltaTime)` 根据速度更新位置；
4. `CollisionSystem::Update(world)` 检查实体之间的碰撞；
5. `PrintPosition()` 输出玩家当前位置；
6. 继续等待下一次输入，直到玩家退出或输入流失败。

移动使用以下公式：

```text
新位置 = 旧位置 + 速度 × deltaTime
```

## 项目目录

```text
MiniGameRuntime/
├─ CMakeLists.txt
├─ MiniGameRuntime/
│  ├─ CMakeLists.txt
│  ├─ docs/
│  ├─ include/
│  │  ├─ Common/
│  │  ├─ Components/
│  │  ├─ Core/
│  │  ├─ Gameplay/
│  │  ├─ Math/
│  │  ├─ Physics/
│  │  └─ Systems/
│  └─ src/
│     ├─ Core/
│     ├─ Gameplay/
│     ├─ Systems/
│     └─ main.cpp
└─ README.md
```

目录中还保留了部分学习演示和 Benchmark 文件；当前最终 Sandbox 的入口是 `MiniGameRuntime/src/main.cpp`。

## 构建与运行

### 环境要求

- 支持 C++17 或更高标准的编译器；
- CMake；
- Visual Studio 2022（Windows 环境推荐）。

### 使用 Visual Studio

1. 使用 Visual Studio 打开仓库根目录；
2. 等待 CMake 配置完成；
3. 选择 `MiniGameRuntime.exe` 作为启动项；
4. 生成并运行项目。

### 使用命令行

在仓库根目录执行：

```powershell
cmake -S . -B out/build/x64-debug
cmake --build out/build/x64-debug --config Debug
```

构建完成后运行：

```powershell
.\out\build\x64-debug\bin\MiniGameRuntime.exe
```

不同 CMake 生成器可能会产生略有差异的可执行文件路径，请以实际构建输出为准。

## 测试内容

阶段四最终版本已对以下行为进行手动测试：

- 项目能够重新生成、编译并正常运行；
- `W/A/S/D` 能够改变玩家移动方向；
- `Q` 能够正常退出；
- 非法字符不会执行世界更新；
- 输入流失败时程序能够安全结束；
- 玩家接近墙体后能够触发碰撞检测；
- 没有速度组件的静态实体不会移动；
- 销毁实体后，其位置、速度和 AABB 组件会被清理；
- 重复销毁同一个实体不会破坏程序状态。

详细测试结果见 `MiniGameRuntime/docs/Stage4_Test_Report.md`。

## 当前限制

- 项目仍然是控制台程序，没有图形窗口和实时渲染；
- 输入以控制台字符为单位，不是连续键盘状态输入；
- 碰撞系统目前主要负责检测和输出结果，没有实现完整的碰撞响应；
- 玩家可以继续穿过发生碰撞的静态物体；
- 组件类型和系统数量较少，还不是通用 ECS；
- 尚未实现资源管理、场景管理、音频和脚本等完整引擎功能。

## 学习成果

通过本阶段项目，主要练习了以下内容：

- `struct`、`class`、引用、指针和 `const`；
- `std::vector` 与 `std::unordered_map`；
- 实体 ID 和组件数据分离；
- 头文件与源文件拆分；
- CMake 项目构建；
- 逐帧更新与 `deltaTime`；
- AABB 碰撞检测；
- 系统职责划分；
- Git 分支、提交和阶段性版本管理。

## 后续方向

阶段四结束后，下一步将进入窗口与图形基础阶段，逐步学习 SDL2 和 OpenGL，并把当前控制台 Sandbox 中的实体、移动与碰撞逻辑迁移到可视化程序中。

## 项目状态

阶段四 Sandbox：已完成。

