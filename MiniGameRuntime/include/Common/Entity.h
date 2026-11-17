#pragma once
#include <cstdint>

// 给 uint32_t 起一个别名叫 Entity，让代码可读性更强
using Entity = std::uint32_t;

// 定义一个无效的 ID，用来做安全检查（比如找不到某个实体时返回它）
constexpr Entity INVALID_ENTITY = 0;