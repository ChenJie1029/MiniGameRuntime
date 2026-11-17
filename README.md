# MiniGameRuntime v2

MiniGameRuntime v2 是一个 Windows x64 的 2D 游戏运行时演示，使用 C++20、SDL2、OpenGL 3.3 Core、GLAD 和 stb_image。场景里有玩家、墙、自动移动的小图和一个收集物。玩家撞墙时撤回本帧的移动，拾取物品后获得 1 分。

## 构建和运行

- Windows x64，显卡驱动支持 OpenGL 3.3 Core。
- Visual Studio 2022，安装「使用 C++ 的桌面开发」和 CMake 工具。
- CMake 配置依赖时使用 Git 获取 SDL2 和 GLAD。生成 GLAD 代码需要可用的 Python 3 和 Jinja2。

**Visual Studio** 打开仓库根目录，选择 `x64 Debug` 配置，等待 CMake 配置完成，然后以 `MiniGameRuntime.exe` 为启动目标构建并运行。

**命令行** 在能找到 CMake 和 Ninja 的 Visual Studio 2022 x64 Native Tools 命令提示符中，进入仓库根目录后执行以下命令。

```bat
cmake --preset x64-debug
cmake --build out/build/x64-debug --target MiniGameRuntime
.\out\build\x64-debug\bin\MiniGameRuntime.exe
```

## 操作

先让游戏窗口获得焦点。

| 按键 | 功能 |
| --- | --- |
| `W / A / S / D` | 移动玩家 |
| 方向键 | 移动镜头 |
| `P` | 暂停或恢复 |
| `R` | 重新开始，重置镜头和分数 |
| `Delete` | 销毁玩家 |
| `F5` | 保存玩家位置和分数 |
| `F9` | 读取存档 |
| `Esc` 或窗口关闭按钮 | 退出 |

启动后按住 `S` 可拾取玩家下方的物品。窗口标题约每秒更新一次，显示 FPS、平均帧耗时、分数和暂停状态。

## 存档

`F5` 在程序的工作目录中写入 `scene_save.json`，控制台会打印它的完整路径。文件记录玩家的 `x`、`y` 和分数。读取时按初始布局重建场景，再恢复玩家位置和收集进度。分数为 `1` 时，收集物会被移除。

## 代码入口

- 主循环处理动作请求，依次调用更新和绘制。 [main.cpp](MiniGameRuntime/src/main.cpp)
- 场景创建实体，处理重开和存读档。 [Scene.cpp](MiniGameRuntime/src/Core/Scene.cpp)
- 世界管理实体组件，查询待绘制的实体。 [World.cpp](MiniGameRuntime/src/Core/World.cpp)
- 移动系统按速度和帧时间更新位置。碰撞系统检查实体的 AABB 是否重叠。 [移动系统](MiniGameRuntime/src/Systems/MovementSystem.cpp) · [碰撞系统](MiniGameRuntime/src/Systems/CollisionSystem.cpp)
- 纹理和着色器程序由各自的类管理生命周期。 [Texture2D](MiniGameRuntime/src/Resources/Texture2D.cpp) · [ShaderProgram](MiniGameRuntime/src/Resources/ShaderProgram.cpp)

## 前两阶段资料

- [阶段 4 验收记录](MiniGameRuntime/docs/Stage4_Test_Report.md)
- [阶段 5 渲染总结](MiniGameRuntime/docs/Stage5_Rendering_Summary.md)
