# ============================================
# 1. 指定 CMake 最低版本
# ============================================
cmake_minimum_required(VERSION 3.10)

# ============================================
# 2. 定义项目名称
# ============================================
project(MiniGameRuntime)

# ============================================
# 3. 设置 C++ 标准
# ============================================
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ============================================
# 4. 添加可执行文件（只列源文件 .cpp）
# ============================================
add_executable(MiniGameRuntime 
    MiniGameRuntime.cpp
    src/main.cpp
    src/Vec2.cpp
 )

# ============================================
# 5. 指定头文件搜索路径
# ============================================
target_include_directories(MiniGameRuntime PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}        # 根目录（找 MiniGameRuntime.h）
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# ============================================
# 6. 可选：设置输出目录
# ============================================
set_target_properties(MiniGameRuntime PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# ============================================
# 7. 可选：打印构建信息（方便调试）
# ============================================
message(STATUS "Building ${PROJECT_NAME} with C++${CMAKE_CXX_STANDARD}")
message(STATUS "Source files: MiniGameRuntime.cpp, src/main.cpp, src/Vec2.cpp")
