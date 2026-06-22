#include <iostream>

using namespace std;

struct Vec2 {
    float x;
    float y;

    Vec2();
    Vec2(float xValue, float yValue);

    Vec2 Add(const Vec2& other) const;
    Vec2 Subtract(const Vec2& other) const;
    Vec2 Multiply(float scalar) const;
};

int main() {
    cout << "MiniGameRuntime started." << endl;

    return 0;
}