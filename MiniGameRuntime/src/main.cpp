#include <iostream>
#include "Vec2.h"

using namespace std;

int main() {
    cout << "MiniGameRuntime started." << endl;

    //调用带参构造函数创建a, b
    Vec2 a(1.0f, 2.0f);
    Vec2 b(3.0f, 4.0f);

    //调用带参构造函数创建c, 但是c又调用了成员方法
    Vec2 c = a.Add(b);
    Vec2 d = a.Subtract(b);
    Vec2 e = a.Multiply(2.0);

    cout << "c.x:" << c.x << " " << "c.y:" << c.y << endl;
    cout << "d.x:" << d.x << " " << "d.y:" << d.y << endl;
    cout << "e.x:" << e.x << " " << "e.y:" << e.y << endl;

    return 0;
}