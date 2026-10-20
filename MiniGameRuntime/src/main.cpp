#include <iostream>

#include <glad/gl.h>
#include <SDL.h>

constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGHT = 600;

int main(int argc, char* argv[])
{
    // ----------添加顶点着色器----------
    const char* vertexShaderSource = R"glsl(
#version 330 core

layout(location = 0) in vec3 position;

void main()
{
    gl_Position = vec4(position, 1.0);
}
)glsl";
    // #version 330 core   使用GLSL 3.30核心模式，与OpenGL 3.3对应
    // layout(location = 0) in vec3 position;   从VAO的属性位置0读取三个数 x、y、z
    // gl_Position = vec4(position, 1.0);   将输入位置设置为顶点最终位置  gl_Position 要求四个分量x、y、z、w  所以将vec3转换成vec4(position, 1.0)

    // ----------添加片段着色器----------
    const char* fragmentShaderSource = R"glsl(
#version 330 core

out vec4 fragmentColor;

void main()
{
    fragmentColor = vec4(0.2, 1.0, 0.3, 1.0);
}
)glsl";
    // out vec4 fragmentColor;   声明最终输出颜色
    // vec4(1.0, 0.5, 0.2, 1.0)  红 = 1.0，绿 = 0.5，蓝 = 0.2，透明度 = 1.0
    // R"glsl(... )glsl" 是C++原始字符串，使我们可以直接在C++中保存多行着色器代码

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

    // ----------编译顶点着色器----------
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER); // 创建顶点着色器对象

    glShaderSource( // 把源代码交给着色器对象
        vertexShader,
        1,
        &vertexShaderSource,
        nullptr
    );

    glCompileShader(vertexShader); //编译GPU程序 这是编译GLSL着色器，不是编译C++

    // ----------编译片段着色器----------
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(
        fragmentShader,
        1,
        &fragmentShaderSource,
        nullptr
    );

    glCompileShader(fragmentShader);

    // ----------链接Shader Program----------
    GLuint shaderProgram = glCreateProgram(); // 创建着色器程序
    glAttachShader(shaderProgram, vertexShader); // 把顶点着色器安装到程序上
    glAttachShader(shaderProgram, fragmentShader); // 把片段着色器安装到程序上
    glLinkProgram(shaderProgram); // 将它们连接成完整的GPU绘制程序

    // ----------删除单独的着色器对象----------
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

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

    constexpr GLsizei vertexCount =
        static_cast<GLsizei>(
            sizeof(triangleVertices) /
            sizeof(triangleVertices[0]) /
            valuesPerVertex
            );

    // GLuint：OpenGL 用来表示对象编号的类型
    GLuint vertexArrayObject = 0; // VAO:记录“顶点数据应该怎样读取”的配置
    GLuint vertexBufferObject = 0; // VBO:显卡中的一块缓冲区，真正保存顶点数据 

    glGenVertexArrays(1, &vertexArrayObject);
    glGenBuffers(1, &vertexBufferObject);

    // 绑定并上传数据
    glBindVertexArray(vertexArrayObject);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);

    glBufferData(
        GL_ARRAY_BUFFER, // 上传的是顶点数据
        sizeof(triangleVertices), // 数据总字节数
        triangleVertices, // CPU中的原始数据
        GL_STATIC_DRAW // 上传后通常不会频繁修改
    );

    // 顶点排列方式
    glVertexAttribPointer(
        0, // 顶点属性位置 location = 0
        valuesPerVertex, // 每个位置由x,y,z三个数构成
        GL_FLOAT, // 每个数都是float
        GL_FALSE, // 不进行归一化转换
        valuesPerVertex * sizeof(float), // 相邻两个顶点之间的字节距离
        nullptr // 第一个属性从缓冲区第0字节开始
    );

    glEnableVertexAttribArray(0);

    // 解除绑定
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // 输出对象编号
    std::cout << "VAO: " << vertexArrayObject << std::endl;
    std::cout << "VBO: " << vertexBufferObject << std::endl;
    std::cout << "Vertex count: " << vertexCount << std::endl;

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

        glUseProgram(shaderProgram); // 选择着色器程序  告诉OpenGL接下来的绘制使用这个着色器程序
        glBindVertexArray(vertexArrayObject); // 选择VAO 恢复VAO中保存的顶点读取规则 VAO会告诉OpenGL：位置数据来自哪个VBO、每个顶点包含3个float、属性位置为0
        glDrawArrays(GL_TRIANGLES, 0, vertexCount); // 发出绘制命令：按三角形方式绘制、从第0个顶点开始、一共读取3个顶点
        glBindVertexArray(0); // 解除VAO绑定

        // 将OpenGL绘制结果显示到窗口
        SDL_GL_SwapWindow(window);
    }

    // 释放Shader Program
    glUseProgram(0);
    glDeleteProgram(shaderProgram);

    // 释放OpenGL对象
    glDeleteBuffers(1, &vertexBufferObject);
    glDeleteVertexArrays(1, &vertexArrayObject);

    SDL_GL_DeleteContext(openGLContext);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}