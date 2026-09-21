#include <iostream>

#include "Math/Vec2.h"

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

int main()
{
    // ======================================================================
    std::cout << "======================================================================" << std::endl;

    Vec2 position{ 10.0f, 5.0f };
    Vec2 velocity{ 4.0f, -2.0f };

    constexpr float deltaTime = 0.5f;

    std::cout << "Initial position: ";
    PrintVec2(position);

    std::cout << "Velocity: ";
    PrintVec2(velocity);

    std::cout << "Delta time: " << deltaTime << std::endl;

    for (int frame = 1; frame <= 3; frame++) {
        Move(position, velocity, deltaTime);
        std::cout << "Frame: " << frame << std::endl;
        PrintVec2(position);
    }

    return 0;
}