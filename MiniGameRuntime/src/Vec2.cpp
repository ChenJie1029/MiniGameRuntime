#include "Vec2.h"

//默认构造函数实现
Vec2::Vec2() : x(0.0f), y(0.0f) {}
//Vec2::  作用域解析符，表示这是 Vec2 的成员函数
//Vec2()  构造函数名，与结构体同名
//: x(0.0f), y(0.0f)  初始化列表，直接初始化 x 和 y
//{}  函数体为空（初始化已经在初始化列表中完成了）
// 
//等价写法：
//Vec2::Vec2() {
//    x = 0.0f;
//    y = 0.0f;
//}

//============================================
//带参构造函数实现
Vec2::Vec2(float xValue, float yValue) : x(xValue), y(yValue) {}

//============================================
//向量加法实现
Vec2 Vec2::Add(const Vec2& other) const {
	return Vec2(x + other.x, y + other.y);
}

//============================================
//向量减法实现
Vec2 Vec2::Subtract(const Vec2& other) const {
	return Vec2(x - other.x, y - other.y);
}

//============================================
//向量乘法实现
Vec2 Vec2::Multiply(float scalar) const {
	return Vec2(x * scalar, y * scalar);
}
