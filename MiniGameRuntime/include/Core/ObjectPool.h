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

		~ObjectPool() = default;//

		// 借出
		T* Acquire() { // 我们要自定义一个名叫Acquire的成员函数，它不需要传参，执行完后会把一个T*类型的内存地址交出来
			if (m_activeCount >= m_pool.size()) { // 池子满了
				throw std::runtime_error("[ObjectPool] Pool is completely full! Cannot acquire new object.");
			}

			T& obj = m_pool[m_activeCount]; // 拿到分界线上的那个空闲对象（它正好处于m_activeCount的下标位置）

			m_activeCount++; // 游标右移，代表繁忙的人数 +1，这个对象被正式划归到“繁忙区”

			obj.Init(); // 擦洗并初始化

			return &obj;
		}

		// 归还
		void Release(T* target) {
			if (target == nullptr || m_activeCount == 0) return;

			size_t targetIndex = target - m_pool.data(); // target指针减去数组首地址指针，就能算出它是第几个元素
			// .data() 指向数组里第一个元素（即 m_pool[0]）在内存里的地址

			if (targetIndex >= m_pool.size() || targetIndex >= m_activeCount) {
				std::cout << "[ObjectPool] Warning: Attempted to release an invalid or already inactive object!" << std::endl;
				return;
			}

			m_pool[targetIndex].Reset(); // 调用 Reset，把这个对象的肉身属性擦干净

			size_t lastActiveIndex = m_activeCount - 1;
			if (targetIndex != lastActiveIndex) {
				std::swap(m_pool[targetIndex], m_pool[lastActiveIndex]);
			}

			m_activeCount--;
		}

		//辅助函数：获取当前状态
		size_t GetActiveCount() const { return m_activeCount; }
		size_t GetCapacity() const { return m_pool.size(); }

	private:
		std::vector<T> m_pool; // 对象池的物理连续大仓库
		size_t m_activeCount; // 游标卡尺：划分繁忙与空闲的边界
	};
}
