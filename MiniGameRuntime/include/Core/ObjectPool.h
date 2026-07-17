#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <stdexcept>

namespace Hazel {
	class IPoolable {
	public:
		virtual ~IPoolable() = default;
		virtual void Init() = 0;
		virtual void Reset() = 0;
	};

	template<typename T>
	class ObjectPool {
		//编译时，如果编译不通过报错
		static_assert(std::is_base_of<IPoolable, T>::value, "Template type T must inherit from IPoolable");

	public:
		explicit ObjectPool(size_t initialCapacity) { // explicit 显示关键字
			m_pool.resize(initialCapacity);
			m_activeCount = 0;
			std::cout << "[ObjectPool] Initialized with capacity: " << initialCapacity << std::endl;
		}

		~ObjectPool() = default;
	};
}
