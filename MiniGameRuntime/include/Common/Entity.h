#pragma once
#include <cstdint>

using namespace std;

// 给 uint32_t 起一个别名叫 Entity，让代码可读性更强
using Entity = uint32_t;

// 定义一个无效的 ID，用来做安全检查（比如找不到某个实体时返回它）
const Entity INVALID_ENTITY = 0;