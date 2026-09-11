# MiniGameRuntime 第一层结构

## 1. 程序入口

入口文件：main.cpp

main()目前的作用：启动程序，创建实例

## 2. Entity

Entity的真实类型：uint32_t

Entity保存的是完整角色数据，还是一个ID：只保存一个ID

INVALID_ENTITY的作用：定义无效ID用来做安全检查

## 3. EntityManager

EntityManager负责：管理实体，创建实体所需的函数

它保存的两个主要成员变量：nextID、vector存放实体的容器

## 4. World

World负责：汇总组件接口，位置、速度、碰撞

World内部保存的三个组件容器：位置、速度、碰撞

World与EntityManager的区别：EntityManager管理实体更加的偏向成员变量和成员函数偏向静态、World更像是一个组件的汇总偏向动态

## 5. 一帧移动流程

main()
→ 
→ 
→ 
→ 输出新位置

移动公式：新位置 = 旧位置 + 速度 * 帧数

## 6. 当前发现的结构问题

1.


main()
→ 调用world.Update(deltaTime)
→ World遍历 位置GetTransform 组件
→ 使用Entity ID查找 位置 组件
→ 计算 组件位置
→ main()调用GetTransform()
→ 输出新位置