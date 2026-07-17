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

	//
}
