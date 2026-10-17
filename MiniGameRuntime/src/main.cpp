#include <iostream>
#include <cmath>
#include <SDL.h>
#include <string>
#include <cmath>
#include <vector>

#include "Math/Vec2.h"

struct Mat3 {
    float m[3][3];
};

struct Wall {
    Vec2 position;
    float width;
    float height;
};

Vec2 CalculateDisplacement(const Vec2& velocity, float deltaTime) {
    return velocity * deltaTime;
}

void Move(Vec2& position, const Vec2& velocity, float deltaTime) {
    Vec2 displacement = CalculateDisplacement(velocity, deltaTime);
    position = position + displacement;
}

void PrintVec2(const Vec2& vector) {
    std::cout << "(" << vector.x << ", " << vector.y << ")" << std::endl;
}

constexpr float PI = 3.14159265358979323846f;

float DegreesToRadians(float degrees) { // 角度转弧度
    return degrees * (PI / 180);
}

Vec2 DirectionFromDegrees(float degrees) { // 根据角度计算方向
    float radians = DegreesToRadians(degrees);

    Vec2 direction;
    direction.x = std::cos(radians);
    direction.y = std::sin(radians);

    return direction;
}

Vec2 RotateVector(const Vec2& vector, float degrees) { // 旋转二维向量
    float radians = DegreesToRadians(degrees);
    float cosine = std::cos(radians);
    float sine = std::sin(radians);

    Vec2 rotated;

    rotated.x = vector.x * cosine - vector.y * sine;
    rotated.y = vector.x * sine + vector.y * cosine;

    return rotated;
}

Vec2 TranslatePoint(const Vec2& point, const Vec2& translation) { // 平移
    return point + translation;
}

Vec2 ScalePoint(const Vec2& point, const Vec2& scale) { // 缩放
    Vec2 newPoint;

    newPoint.x = point.x * scale.x;
    newPoint.y = point.y * scale.y;

    return newPoint;
}

Vec2 TransformPoint(const Vec2& localPoint, const Vec2& position, const Vec2& scale, float rotationDegrees) {
    Vec2 transformed = ScalePoint(localPoint, scale);
    transformed = RotateVector(transformed, rotationDegrees);
    transformed = TranslatePoint(transformed, position);

    return transformed;
}

Vec2 TransformPointByMatrix(const Mat3& matrix, const Vec2& point) { // 矩阵变换一个点  参数列表（操作矩阵，被操作对象 坐标）
    Vec2 result;

    result.x = matrix.m[0][0] * point.x + matrix.m[0][1] * point.y + matrix.m[0][2] * 1.0f;
    result.y = matrix.m[1][0] * point.x + matrix.m[1][1] * point.y + matrix.m[1][2] * 1.0f;

    return result;
}

Mat3 CreateScaleMatrix(float scaleX, float scaleY) { // 创建缩放矩阵
    Mat3 matrix{};

    matrix.m[0][0] = scaleX;
    matrix.m[1][1] = scaleY;
    matrix.m[2][2] = 1.0f;

    return matrix;
}

Mat3 CreateRotationMatrix(float degrees) { // 创建旋转矩阵
    float radians = DegreesToRadians(degrees);
    float cosine = std::cos(radians);
    float sine = std::sin(radians);

    Mat3 matrix{};

    matrix.m[0][0] = cosine;
    matrix.m[0][1] = -sine;
    matrix.m[1][0] = sine;
    matrix.m[1][1] = cosine;
    matrix.m[2][2] = 1.0f;

    return matrix;
}

Mat3 CreateTranslationMatrix(float translationX, float translationY) { // 创建平移矩阵
    Mat3 matrix{};

    matrix.m[0][0] = 1.0f;
    matrix.m[1][1] = 1.0f;
    matrix.m[2][2] = 1.0f;
    matrix.m[0][2] = translationX;
    matrix.m[1][2] = translationY;

    return matrix;
}

Vec2 WorldToView(const Vec2& worldPoint, const Vec2& cameraPosition) { // 世界坐标转观察坐标
    return worldPoint - cameraPosition;
}

Vec2 ViewToNDC(const Vec2& viewPoint, float viewWidth, float viewHeight) { // 观察坐标转NDC坐标  假设摄像机可见范围宽800、高600
    Vec2 ndc;

    ndc.x = viewPoint.x / (viewWidth * 0.5f);
    ndc.y = viewPoint.y / (viewHeight * 0.5f);

    return ndc;
}

Vec2 NDCToScreen(const Vec2& ndc, float screenWidth, float screenHeight) { // NDC转屏幕坐标
    Vec2 screen;

    screen.x = (ndc.x + 1.0f) * 0.5f * screenWidth;
    screen.y = (1.0f - ndc.y) * 0.5f * screenHeight;

    return screen;
}

//int main()
//{
    // ======================================================================
    //std::cout << "======================================================================" << std::endl;

    //Vec2 position{ 10.0f, 5.0f };
    //Vec2 velocity{ 4.0f, -2.0f };

    //constexpr float deltaTime = 0.5f;

    //std::cout << "Initial position: ";
    //PrintVec2(position);

    //std::cout << "Velocity: ";
    //PrintVec2(velocity);

    //std::cout << "Delta time: " << deltaTime << std::endl;

    //for (int frame = 1; frame <= 3; frame++) {
    //    Move(position, velocity, deltaTime);
    //    std::cout << "Frame: " << frame << std::endl;
    //    PrintVec2(position);
    //}

    //std::cout << DegreesToRadians(180.0f) << std::endl;

    // ==================
    //std::cout << "==================" << std::endl;

    //PrintVec2(DirectionFromDegrees(0.0f));
    //PrintVec2(DirectionFromDegrees(90.0f));
    //PrintVec2(DirectionFromDegrees(180.0f));
    //PrintVec2(DirectionFromDegrees(270.0f));

    //// ==================
    //std::cout << "==================" << std::endl;

    //Vec2 original{ 2.0f, 0.0f };

    //Vec2 rotated90 = RotateVector(original, 90.0f);
    //Vec2 rotated180 = RotateVector(original, 180.0f);

    //PrintVec2(rotated90);
    //PrintVec2(rotated180);

    // ======================================================================
    //std::cout << "===================================Day 3===================================" << std::endl;

    //Vec2 localPoint{ 2.0f, 0.0f };
    //Vec2 objectPosition{ 10.0f, 5.0f };
    //Vec2 objectScale{ 2.0f, 1.0f };
    //float objectRotation = 90.0f;

    //Vec2 scaled = ScalePoint(localPoint, objectScale);
    //Vec2 rotated = RotateVector(scaled, objectRotation);
    //Vec2 worldPoint = TranslatePoint(rotated, objectPosition);

    //Vec2 transformed = TransformPoint(
    //    localPoint,
    //    objectPosition,
    //    objectScale,
    //    objectRotation
    //);

    //std::cout << "Local point: ";
    //PrintVec2(localPoint);

    //std::cout << "Scaled point: ";
    //PrintVec2(scaled);

    //std::cout << "Rotated point: ";
    //PrintVec2(rotated);

    //std::cout << "World point: ";
    //PrintVec2(worldPoint);

    //std::cout << "TransformPoint result: ";
    //PrintVec2(transformed);

    // ======================================================================
    //std::cout << "===================================Day 4===================================" << std::endl;

    //Vec2 localPoint{ 2.0f, 0.0f };
    //Mat3 scaleMatrix = CreateScaleMatrix(2.0f, 1.0f);
    //Mat3 rotationMatrix = CreateRotationMatrix(90.0f);
    //Mat3 translationMatrix = CreateTranslationMatrix(10.0f, 5.0f);

    //Vec2 matrixResult = TransformPointByMatrix(scaleMatrix, localPoint);
    //matrixResult = TransformPointByMatrix(rotationMatrix, matrixResult);
    //matrixResult = TransformPointByMatrix(translationMatrix, matrixResult);

    //Vec2 wrongOrder = TransformPointByMatrix(translationMatrix, localPoint);
    //wrongOrder = TransformPointByMatrix(rotationMatrix, wrongOrder);
    //wrongOrder = TransformPointByMatrix(scaleMatrix, wrongOrder);

    //std::cout << "Wrong order result: ";
    //PrintVec2(wrongOrder);

    //std::cout << "Matrix result: ";
    //PrintVec2(matrixResult);

    // ======================================================================
    //std::cout << "===================================Day 5===================================" << std::endl;

    //Vec2 localPoint{ 0.0f, 0.0f };

    //Vec2 objectPosition{ 300.0f, 200.0f };
    //Vec2 objectScale{ 1.0f, 1.0f };
    //float objectRotation = 0.0f;

    //Vec2 cameraPosition{ 100.0f, 50.0f };

    //Vec2 centerWorldPoint{ 100.0f, 50.0f };

    //constexpr float viewWidth = 800.0f;
    //constexpr float viewHeight = 600.0f;

    //constexpr float screenWidth = 800.0f;
    //constexpr float screenHeight = 600.0f;

    //Vec2 worldPoint = TransformPoint(
    //    localPoint,
    //    objectPosition,
    //    objectScale,
    //    objectRotation
    //);

    //Vec2 viewPoint = WorldToView(worldPoint, cameraPosition);
    //Vec2 ndcPoint = ViewToNDC(viewPoint, viewWidth, viewHeight);
    //Vec2 screenPoint = NDCToScreen(ndcPoint, screenWidth, screenHeight);

    //std::cout << "Local: ";
    //PrintVec2(localPoint);

    //std::cout << "World: ";
    //PrintVec2(worldPoint);

    //std::cout << "View: ";
    //PrintVec2(viewPoint);

    //std::cout << "NDC: ";
    //PrintVec2(ndcPoint);

    //std::cout << "Screen: ";
    //PrintVec2(screenPoint);

    //Vec2 centerViewPoint = WorldToView(centerWorldPoint, cameraPosition);

    //Vec2 centerNDCPoint = ViewToNDC(centerViewPoint, viewWidth, viewHeight);

    //Vec2 centerScreenPoint = NDCToScreen(centerNDCPoint, screenWidth, screenHeight);

    //std::cout << "Center View: ";
    //PrintVec2(centerViewPoint);

    //std::cout << "Center NDC: ";
    //PrintVec2(centerNDCPoint);

    //std::cout << "Center Screen: ";
    //PrintVec2(centerScreenPoint);

    //return 0;
//}

constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGHT = 600;

constexpr int PLAYER_WIDTH = 50;
constexpr int PLAYER_HEIGHT = 50;

// 墙体大小
constexpr int WALL_WIDTH = 80;
constexpr int WALL_HEIGHT = 200;

float CalculateDeltaTime(Uint64& previousCounter, Uint64 performanceFrequency);

bool ProcessInput(Vec2& direction);

void UpdatePlayer(Vec2& playerPosition, const Vec2& direction, float playerSpeed, float deltaTime);

void Render(SDL_Renderer* renderer, const Vec2& playerPosition, const std::vector<Wall>& walls, bool playerCollided);

void KeepPlayerInsideWindow(Vec2& playerPosition);

float Length(const Vec2& vector) { // 斜方向位移
    return std::sqrt(vector.x * vector.x + vector.y * vector.y);
}

Vec2 Normalize(const Vec2& vector) { // 归一化函数
    float length = Length(vector);

    if (length <= 0.0f) {
        return Vec2{ 0.0f, 0.0f };
    }

    return Vec2{ vector.x / length, vector.y / length };
}

float CalculateDeltaTime(Uint64& previousCounter, Uint64 performanceFrequency) { // 上一次时间点 参数值每秒多少tick
    Uint64 currentCounter = SDL_GetPerformanceCounter(); // 读取当前时刻的高精度计时器读数，并存到一个 64 位无符号整数变量里

    float deltaTime = static_cast<float>(currentCounter - previousCounter) / static_cast<float>(performanceFrequency); // (距上次调用过了多少 tick) / 过了多少秒

    previousCounter = currentCounter; // 

    return deltaTime; // 从上一帧到这一帧，经过了多少时间
}

bool ProcessInput(Vec2& direction) {
    direction = Vec2{ 0.0f, 0.0f }; // 方向

    SDL_Event event; // 具体键盘事件

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            return false;
        }

        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
            return false;
        }
    }

    const Uint8* keyboardState = SDL_GetKeyboardState(nullptr);

    if (keyboardState[SDL_SCANCODE_W]) {
        direction.y -= 1.0f;
    }

    if (keyboardState[SDL_SCANCODE_A]) {
        direction.x -= 1.0f;
    }

    if (keyboardState[SDL_SCANCODE_S]) {
        direction.y += 1.0f;
    }

    if (keyboardState[SDL_SCANCODE_D]) {
        direction.x += 1.0f;
    }

    direction = Normalize(direction);

    return true;
}

void UpdatePlayer(Vec2& playerPosition, const Vec2& direction, float playerSpeed, float deltaTime) {
    playerPosition.x += direction.x * playerSpeed * deltaTime;
    playerPosition.y += direction.y * playerSpeed * deltaTime;
}

void Render(SDL_Renderer* renderer, const Vec2& playerPosition, const std::vector<Wall>& walls, bool playerCollided) {
    SDL_SetRenderDrawColor(renderer, 30, 30, 40, 255); // 背景色
    SDL_RenderClear(renderer); // 清屏

    SDL_SetRenderDrawColor(renderer, 160, 160, 160, 255);
    for (const Wall& wall : walls) {
        SDL_Rect wallRect{
            static_cast<int>(wall.position.x - wall.width * 0.5f),
            static_cast<int>(wall.position.y - wall.height * 0.5f),
            static_cast<int>(wall.width),
            static_cast<int>(wall.height)
        };

        SDL_RenderFillRect(renderer, &wallRect);
    }

    constexpr int playerWidth = 50;
    constexpr int playerHeight = 50;

    SDL_Rect playerRect{ 
        static_cast<int>(playerPosition.x) - playerWidth / 2, // 矩形左上角坐标X部分
        static_cast<int>(playerPosition.y) - playerHeight / 2, // 矩形左上角坐标Y部分
        playerWidth, // 宽
        playerHeight // 高
    };

    if (playerCollided) {
        // 碰撞时显示红色
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    }
    else {
        // 正常状态显示黄色
        SDL_SetRenderDrawColor(renderer, 255, 220, 60, 255);
    }

    SDL_RenderFillRect(renderer, &playerRect); // 填充

    SDL_RenderPresent(renderer); // 提交 显示到屏幕
}

float Clamp(float value, float minValue, float maxValue) {
    if (value < minValue) {
        return minValue;
    }

    if (value > maxValue) {
        return maxValue;
    }

    return value;
}

void KeepPlayerInsideWindow(Vec2& playerPosition) {
    float halfWidth = PLAYER_WIDTH * 0.5f;
    float halfHeight = PLAYER_HEIGHT * 0.5f;

    float minX = halfWidth;
    float maxX = WINDOW_WIDTH - halfWidth;

    float minY = halfHeight;
    float maxY = WINDOW_HEIGHT - halfHeight;

    playerPosition.x = Clamp(playerPosition.x, minX, maxX);
    playerPosition.y = Clamp(playerPosition.y, minY, maxY);
}

bool IsColliding(const Vec2& firstPosition, float firstWidth, float firstHeight, const Vec2& secondPosition, float secondWidth, float secondHeight) {
    float distanceX = std::abs(firstPosition.x - secondPosition.x); // 两者X位置距离差值
    float distanceY = std::abs(firstPosition.y - secondPosition.y);

    float combinedHalfWidths = firstWidth * 0.5f + secondWidth * 0.5f; // first的半宽 + second的半宽
    float combinedHalfHeights = firstHeight * 0.5f + secondHeight * 0.5f;

    return distanceX < combinedHalfWidths && distanceY < combinedHalfHeights; // 两者X位置距离差值 < first的半宽 + second的半宽, Y同理
}

const Wall* FindCollidingWall(const Vec2& playerPosition, const std::vector<Wall>& walls) { // 碰到就返回墙的指针
    for (const Wall& wall : walls) {
        if (IsColliding(playerPosition, PLAYER_WIDTH, PLAYER_HEIGHT, wall.position, wall.width, wall.height)) {
            return &wall;
        }
    }

    return nullptr;
}

bool MovePlayerWithCollision(Vec2& playerPosition, const Vec2& direction, float playerSpeed, float deltaTime, const std::vector<Wall>& walls) {
    bool collided = false;

    float movementX = direction.x * playerSpeed * deltaTime; // 计算本帧X轴位移
    float movementY = direction.y * playerSpeed * deltaTime;

    playerPosition.x += movementX; // 实现X轴的移动操作

    KeepPlayerInsideWindow(playerPosition);

    const Wall* collidedWall = FindCollidingWall(playerPosition, walls);

    if (collidedWall != nullptr) { // 碰到
        collided = true;

        if (movementX > 0.0f) { // 本帧产生位移  玩家向右移动贴到左侧墙
            playerPosition.x = collidedWall->position.x - collidedWall->width * 0.5f - PLAYER_WIDTH * 0.5f;
        }
        else if (movementX < 0.0f) { // 本帧产生位移  玩家左侧贴到墙的右侧 
            playerPosition.x = collidedWall->position.x + collidedWall->width * 0.5f + PLAYER_WIDTH * 0.5f;
        }
    }

    playerPosition.y += movementY;

    KeepPlayerInsideWindow(playerPosition);

    collidedWall = FindCollidingWall(playerPosition, walls);

    if (collidedWall != nullptr) {
        collided = true;

        if (movementY > 0.0f) { // 本帧产生位移  玩家向下移动贴到上侧墙
            playerPosition.y = collidedWall->position.y - collidedWall->height * 0.5f - PLAYER_HEIGHT * 0.5f;
        }
        else if (movementY < 0.0f) { // 本帧产生位移  玩家向上移动贴到下侧墙
            playerPosition.y = collidedWall->position.y + collidedWall->height * 0.5f + PLAYER_HEIGHT * 0.5f;
        }
    }

    return collided;
}

int main(int argc, char* argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cout
            << "SDL initialization failed: "
            << SDL_GetError()
            << std::endl;

        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "MiniGameRuntime",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800,
        600,
        SDL_WINDOW_SHOWN
    );

    if (window == nullptr) {
        std::cout
            << "Window creation failed: "
            << SDL_GetError()
            << std::endl;

        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer( // 为指定窗口创建二维绘图环境，失败时返回空指针
        window,
        -1,
        SDL_RENDERER_ACCELERATED
    );

    if (renderer == nullptr) {
        std::cout
            << "Failed to create renderer: "
            << SDL_GetError()
            << std::endl;

        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    std::cout << "SDL window created successfully!" << std::endl;

    Vec2 playerPosition{ 400.0f, 300.0f };

    std::vector<Wall> walls{ // 墙体位置
        {{600.0f, 300.0f}, 80.0f, 200.0f},
        {{300.0f, 150.0f}, 200.0f, 40.0f},
        {{250.0f, 450.0f}, 250.0f, 40.0f}
    };

    constexpr float playerSpeed = 300.0f;

    const Uint64 performanceFrequency = SDL_GetPerformanceFrequency(); // 读取每秒包含多少个计数
    Uint64 previousCounter = SDL_GetPerformanceCounter(); // 读取当前高精度计数

    bool running = true;

    while (running) {
        float deltaTime = CalculateDeltaTime(
            previousCounter,
            performanceFrequency
        );

        Vec2 direction{ 0.0f, 0.0f };

        running = ProcessInput(direction);

        if (!running) {
            break;
        }

        bool playerCollided = MovePlayerWithCollision(playerPosition, direction, playerSpeed, deltaTime, walls);

        Render(renderer, playerPosition, walls, playerCollided);

        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}