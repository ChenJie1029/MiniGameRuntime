#pragma once

struct Vec2 {
    float x;
    float y;

    Vec2(); //默认构造函数
    Vec2(float xValue, float yValue); //带参构造函数 用传入的 xValue 和 yValue 来初始化 x 和 y

    //成员函数（方法）
    Vec2 operator+(const Vec2& other) const; //向量加法：将当前向量与 other 向量相加，返回一个新的 Vec2 对象
    Vec2 operator-(const Vec2& other) const;
    Vec2 operator*(float scalar) const;
};