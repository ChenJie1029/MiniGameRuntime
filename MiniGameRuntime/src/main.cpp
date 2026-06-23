#include <iostream>
#include "Math/Vec2.h"
#include "Components/TransformComponent.h"
#include "Components/VelocityComponent.h"

using namespace std;

int main() {
    cout << "MiniGameRuntime started." << endl;

    //调用带参构造函数创建a, b
    Vec2 a(1.0f, 2.0f);
    Vec2 b(3.0f, 4.0f);

    //调用带参构造函数创建c, 但是c又调用了成员方法
    Vec2 c = a + b;
    Vec2 d = a - b;
    Vec2 e = a * 2.5f;
    Vec2 g = a + b * 3.5f;

    cout << "c.x:" << c.x << " " << "c.y:" << c.y << endl;
    cout << "d.x:" << d.x << " " << "d.y:" << d.y << endl;
    cout << "e.x:" << e.x << " " << "e.y:" << e.y << endl;
    cout << "g.x:" << g.x << " " << "g.y:" << g.y << endl;

    //===========================================================
    //实现对象可以根据速度变更位置
    TransformComponent transform(Vec2(0.0f, 0.0f));
    VelocityComponent Velocity(Vec2(1.0f, 2.0f));

    float deltaTime = 0.016f;
    transform.position = transform.position + Velocity.velocity * deltaTime;

    cout << "X:" << transform.position.x << " " << "Y:" << transform.position.y << endl;

    return 0;
}