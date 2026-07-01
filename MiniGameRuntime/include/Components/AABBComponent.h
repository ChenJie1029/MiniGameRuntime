#pragma once

#include "Math/Vec2.h"

// 碰撞盒组件：挂上这个组件的实体，就代表它在世界中有了物理实体
struct AABBComponent {
    // 盒子的半宽高 (Width/2, Height/2)
    // 比如一个 32x64 的怪物，它的 halfExtents 就是 (16.0f, 32.0f)
    Vec2 halfExtents;

    // 默认构造函数
    AABBComponent() : halfExtents(0.0f, 0.0f) {}

    // 便利构造函数：传入完整的宽高，内部自动除以 2 变成半宽高
    AABBComponent(float width, float height) : halfExtents(width * 0.5f, height * 0.5f) {}
};