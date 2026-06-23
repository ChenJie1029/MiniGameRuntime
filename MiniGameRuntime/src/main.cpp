#include <iostream>
#include "Math/Vec2.h"
#include "Components/TransformComponent.h"

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



    return 0;
}