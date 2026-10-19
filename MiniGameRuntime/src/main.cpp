#include <iostream>

#include <glad/gl.h>
#include <SDL.h>

constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGHT = 600;

int main(int argc, char* argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cout
            << "SDL initialization failed: "
            << SDL_GetError()
            << std::endl;

        return 1;
    }

    // 要求SDL创建OpenGL 3.3核心模式上下文
    SDL_GL_SetAttribute(
        SDL_GL_CONTEXT_MAJOR_VERSION,
        3
    );

    SDL_GL_SetAttribute(
        SDL_GL_CONTEXT_MINOR_VERSION,
        3
    );

    SDL_GL_SetAttribute(
        SDL_GL_CONTEXT_PROFILE_MASK,
        SDL_GL_CONTEXT_PROFILE_CORE
    );

    SDL_GL_SetAttribute(
        SDL_GL_DOUBLEBUFFER,
        1
    );

    SDL_Window* window = SDL_CreateWindow(
        "MiniGameRuntime - OpenGL",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN // 表示这个窗口将用于 OpenGL 绘制
    );

    if (window == nullptr) {
        std::cout
            << "Window creation failed: "
            << SDL_GetError()
            << std::endl;

        SDL_Quit();
        return 1;
    }

    SDL_GLContext openGLContext =
        SDL_GL_CreateContext(window); // OpenGL与当前窗口之间的绘图环境和状态集合 没有 OpenGL 上下文，就不能在这个窗口中执行 OpenGL 绘制

    if (openGLContext == nullptr) {
        std::cout
            << "OpenGL context creation failed: "
            << SDL_GetError()
            << std::endl;

        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // 开启垂直同步
    if (SDL_GL_SetSwapInterval(1) != 0) {
        std::cout
            << "Warning: VSync could not be enabled: "
            << SDL_GetError()
            << std::endl;
    }

    int openGLVersion = gladLoadGL(
        reinterpret_cast<GLADloadfunc>(
            SDL_GL_GetProcAddress
            )
    );

    if (openGLVersion == 0) {
        std::cout
            << "Failed to initialize GLAD!"
            << std::endl;

        SDL_GL_DeleteContext(openGLContext);
        SDL_DestroyWindow(window);
        SDL_Quit();

        return 1;
    }

    std::cout
        << "Loaded OpenGL "
        << GLAD_VERSION_MAJOR(openGLVersion)
        << "."
        << GLAD_VERSION_MINOR(openGLVersion)
        << std::endl;

    glViewport( // 告诉 OpenGL 最终图像应该输出到窗口中的哪块矩形区域
        0,
        0,
        WINDOW_WIDTH,
        WINDOW_HEIGHT
    );

    constexpr float triangleVertices[] = {
        // X      Y      Z
        -0.5f, -0.5f,  0.0f,  // 左下
         0.5f, -0.5f,  0.0f,  // 右下
         0.0f,  0.5f,  0.0f   // 上方
    };

    constexpr int valuesPerVertex = 3;

    constexpr int vertexCount =
        static_cast<int>(
            sizeof(triangleVertices) /
            sizeof(triangleVertices[0])
            ) / valuesPerVertex;

    std::cout
        << "Triangle vertex count: "
        << vertexCount
        << std::endl;

    bool running = true;

    while (running) {
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            else if (
                event.type == SDL_KEYDOWN &&
                event.key.keysym.sym == SDLK_ESCAPE)
            {
                running = false;
            }
        }

        // 指定OpenGL清屏颜色 OpenGL颜色范围是：0.0 ～ 1.0  红、绿、蓝、透明度
        glClearColor(
            0.12f,
            0.12f,
            0.18f,
            1.0f
        );

        // 清除颜色缓冲区
        glClear(GL_COLOR_BUFFER_BIT);

        // 将OpenGL绘制结果显示到窗口
        SDL_GL_SwapWindow(window);
    }

    SDL_GL_DeleteContext(openGLContext);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}