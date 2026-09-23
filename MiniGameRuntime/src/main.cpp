#include <iostream>
#include <cmath>

#include "Math/Vec2.h"

struct Mat3 {
    float m[3][3];
};

Vec2 CalculateDisplacement(const Vec2& velocity, float deltaTime) {
    return velocity * deltaTime;
}

void Move(Vec2& position, const Vec2& velocity, float deltaTime) {
    Vec2 displacement = CalculateDisplacement(velocity, deltaTime);
    position = position + displacement;
}

void PrintVec2(const Vec2& vector) {
    std::cout << "(" << vector.x << ", " << vector.y << ")" << std::endl;
}

constexpr float PI = 3.14159265358979323846f;

float DegreesToRadians(float degrees) { // 角度转弧度
    return degrees * (PI / 180);
}

Vec2 DirectionFromDegrees(float degrees) { // 根据角度计算方向
    float radians = DegreesToRadians(degrees);

    Vec2 direction;
    direction.x = std::cos(radians);
    direction.y = std::sin(radians);

    return direction;
}

Vec2 RotateVector(const Vec2& vector, float degrees) { // 旋转二维向量
    float radians = DegreesToRadians(degrees);
    float cosine = std::cos(radians);
    float sine = std::sin(radians);

    Vec2 rotated;

    rotated.x = vector.x * cosine - vector.y * sine;
    rotated.y = vector.x * sine + vector.y * cosine;

    return rotated;
}

Vec2 TranslatePoint(const Vec2& point, const Vec2& translation) { // 平移
    return point + translation;
}

Vec2 ScalePoint(const Vec2& point, const Vec2& scale) { // 缩放
    Vec2 newPoint;

    newPoint.x = point.x * scale.x;
    newPoint.y = point.y * scale.y;

    return newPoint;
}

Vec2 TransformPoint(const Vec2& localPoint, const Vec2& position, const Vec2& scale, float rotationDegrees) {
    Vec2 transformed = ScalePoint(localPoint, scale);
    transformed = RotateVector(transformed, rotationDegrees);
    transformed = TranslatePoint(transformed, position);

    return transformed;
}

Vec2 TransformPointByMatrix(const Mat3& matrix, const Vec2& point) { // 矩阵变换一个点  参数列表（操作矩阵，被操作对象 坐标）
    Vec2 result;

    result.x = matrix.m[0][0] * point.x + matrix.m[0][1] * point.y + matrix.m[0][2] * 1.0f;
    result.y = matrix.m[1][0] * point.x + matrix.m[1][1] * point.y + matrix.m[1][2] * 1.0f;

    return result;
}

Mat3 CreateScaleMatrix(float scaleX, float scaleY) { // 创建缩放矩阵
    Mat3 matrix{};

    matrix.m[0][0] = scaleX;
    matrix.m[1][1] = scaleY;
    matrix.m[2][2] = 1.0f;

    return matrix;
}

Mat3 CreateRotationMatrix(float degrees) { // 创建旋转矩阵
    float radians = DegreesToRadians(degrees);
    float cosine = std::cos(radians);
    float sine = std::sin(radians);

    Mat3 matrix{};

    matrix.m[0][0] = cosine;
    matrix.m[0][1] = -sine;
    matrix.m[1][0] = sine;
    matrix.m[1][1] = cosine;
    matrix.m[2][2] = 1.0f;

    return matrix;
}

Mat3 CreateTranslationMatrix(float translationX, float translationY) { // 创建平移矩阵
    Mat3 matrix{};

    matrix.m[0][0] = 1.0f;
    matrix.m[1][1] = 1.0f;
    matrix.m[2][2] = 1.0f;
    matrix.m[0][2] = translationX;
    matrix.m[1][2] = translationY;

    return matrix;
}

int main()
{
    // ======================================================================
    //std::cout << "======================================================================" << std::endl;

    //Vec2 position{ 10.0f, 5.0f };
    //Vec2 velocity{ 4.0f, -2.0f };

    //constexpr float deltaTime = 0.5f;

    //std::cout << "Initial position: ";
    //PrintVec2(position);

    //std::cout << "Velocity: ";
    //PrintVec2(velocity);

    //std::cout << "Delta time: " << deltaTime << std::endl;

    //for (int frame = 1; frame <= 3; frame++) {
    //    Move(position, velocity, deltaTime);
    //    std::cout << "Frame: " << frame << std::endl;
    //    PrintVec2(position);
    //}

    //std::cout << DegreesToRadians(180.0f) << std::endl;

    // ==================
    //std::cout << "==================" << std::endl;

    //PrintVec2(DirectionFromDegrees(0.0f));
    //PrintVec2(DirectionFromDegrees(90.0f));
    //PrintVec2(DirectionFromDegrees(180.0f));
    //PrintVec2(DirectionFromDegrees(270.0f));

    //// ==================
    //std::cout << "==================" << std::endl;

    //Vec2 original{ 2.0f, 0.0f };

    //Vec2 rotated90 = RotateVector(original, 90.0f);
    //Vec2 rotated180 = RotateVector(original, 180.0f);

    //PrintVec2(rotated90);
    //PrintVec2(rotated180);

    // ======================================================================
    //std::cout << "===================================Day 3===================================" << std::endl;

    //Vec2 localPoint{ 2.0f, 0.0f };
    //Vec2 objectPosition{ 10.0f, 5.0f };
    //Vec2 objectScale{ 2.0f, 1.0f };
    //float objectRotation = 90.0f;

    //Vec2 scaled = ScalePoint(localPoint, objectScale);
    //Vec2 rotated = RotateVector(scaled, objectRotation);
    //Vec2 worldPoint = TranslatePoint(rotated, objectPosition);

    //Vec2 transformed = TransformPoint(
    //    localPoint,
    //    objectPosition,
    //    objectScale,
    //    objectRotation
    //);

    //std::cout << "Local point: ";
    //PrintVec2(localPoint);

    //std::cout << "Scaled point: ";
    //PrintVec2(scaled);

    //std::cout << "Rotated point: ";
    //PrintVec2(rotated);

    //std::cout << "World point: ";
    //PrintVec2(worldPoint);

    //std::cout << "TransformPoint result: ";
    //PrintVec2(transformed);

    // ======================================================================
    std::cout << "===================================Day 4===================================" << std::endl;

    Vec2 localPoint{ 2.0f, 0.0f };
    Mat3 scaleMatrix = CreateScaleMatrix(2.0f, 1.0f);
    Mat3 rotationMatrix = CreateRotationMatrix(90.0f);
    Mat3 translationMatrix = CreateTranslationMatrix(10.0f, 5.0f);

    Vec2 matrixResult = TransformPointByMatrix(scaleMatrix, localPoint);
    matrixResult = TransformPointByMatrix(rotationMatrix, matrixResult);
    matrixResult = TransformPointByMatrix(translationMatrix, matrixResult);

    Vec2 wrongOrder = TransformPointByMatrix(translationMatrix, localPoint);
    wrongOrder = TransformPointByMatrix(rotationMatrix, wrongOrder);
    wrongOrder = TransformPointByMatrix(scaleMatrix, wrongOrder);

    std::cout << "Wrong order result: ";
    PrintVec2(wrongOrder);

    std::cout << "Matrix result: ";
    PrintVec2(matrixResult);

    return 0;
}