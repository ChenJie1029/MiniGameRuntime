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
layout(location = 1) in vec3 color;

uniform vec2 objectOffset;

out vec3 vertexColor;

void main()
{
    vec2 movePosition = position.xy + objectOffset;

    gl_Position = vec4(movePosition, position.z, 1.0);
    vertexColor = color;
}
)glsl";
    // #version 330 core   使用GLSL 3.30核心模式，与OpenGL 3.3对应
    // layout(location = 0) in vec3 position;   从VAO的属性位置0读取三个数 x、y、z
    // gl_Position = vec4(position, 1.0);   将输入位置设置为顶点最终位置  gl_Position 要求四个分量x、y、z、w  所以将vec3转换成vec4(position, 1.0)

    // ----------添加片段着色器----------
const char* fragmentShaderSource = R"glsl(
#version 330 core

in vec3 vertexColor;

out vec4 fragmentColor;

void main()
{
    fragmentColor = vec4(vertexColor, 1.0);
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

    // glGetUniformLocation在指定ShaderProgram中查找名为objectOffset的变量，并返回它的位置编号是objectOffsetLocation类型
    GLint objectOffsetLocation = glGetUniformLocation(shaderProgram, "objectOffset"); // 存在于顶点着色器中 uniform vec2 objectOffset;
    // 检查是否找到
    if (objectOffsetLocation == -1) {
        std::cout << "Failed to find objectOffset uniform!" << std::endl;
    }

    // ----------删除单独的着色器对象----------
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glViewport( // 告诉 OpenGL 最终图像应该输出到窗口中的哪块矩形区域
        0,
        0,
        WINDOW_WIDTH,
        WINDOW_HEIGHT
    );

    //constexpr float triangleVertices[] = {
    //    // X      Y      Z
    //    -0.5f, -0.5f,  0.0f,  // 左下
    //     0.5f, -0.5f,  0.0f,  // 右下
    //     0.0f,  0.5f,  0.0f   // 上方
    //};

    //constexpr int valuesPerVertex = 3;

    //constexpr GLsizei vertexCount =
    //    static_cast<GLsizei>(
    //        sizeof(triangleVertices) /
    //        sizeof(triangleVertices[0]) /
    //        valuesPerVertex
    //        );

    constexpr float rectangleVertices[] = {
        // 位置：X      Y      Z    颜色：R   G   B
        -0.5f, -0.35f, 0.0f, 1.0, 0.0f, 0.0f,  // 0: 左下  红
        0.5f, -0.35f, 0.0f, 0.0f, 1.0f, 0.0f,  // 1: 右下  绿
        0.5f, 0.35f, 0.0f, 0.0f, 0.0f, 1.0f,   // 2: 右上  蓝
        -0.5f, 0.35f, 0.0f, 1.0f, 1.0f, 0.0f   // 3: 左上  黄
    };

    // 索引数组 索引中的数字表示 使用的是第几个顶点
    constexpr GLuint rectangleIndices[] = {
        0, 1, 2, // 第一个三角形
        0, 2, 3  // 第二个三角形
    };

    // 计算索引数量 6
    constexpr GLsizei indexCount = static_cast<GLsizei>(sizeof(rectangleIndices) / sizeof(rectangleIndices[0]));

    constexpr int valuesPerVertex = 6;

    constexpr GLsizei vertexStride = valuesPerVertex * sizeof(float); // 从当前顶点开头到移动到下一个顶点开头需要跨过多少字节

    constexpr GLsizei vertexCount =
        static_cast<GLsizei>(
            sizeof(rectangleVertices) /
            sizeof(rectangleVertices[0]) /
            valuesPerVertex
        );

    // GLuint：OpenGL 用来表示对象编号的类型
    GLuint vertexArrayObject = 0; // VAO:记录“顶点数据应该怎样读取”的配置
    GLuint vertexBufferObject = 0; // VBO:显卡中的一块缓冲区，真正保存顶点数据 
    GLuint elementBufferObject = 0; // 创建EBO
    
    // 生成3个OpenGL对象
    glGenVertexArrays(1, &vertexArrayObject); // VAO顶点数据读取规则
    glGenBuffers(1, &vertexBufferObject); // 4个顶点的坐标数据
    glGenBuffers(1, &elementBufferObject); // 组成三角形的顶点编号

    // 绑定并上传数据
    // 选择VAO
    glBindVertexArray(vertexArrayObject);
    // 把顶点坐标上传到VBO
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);

    //glBufferData(
    //    GL_ARRAY_BUFFER, // 上传的是顶点数据
    //    sizeof(triangleVertices), // 数据总字节数
    //    triangleVertices, // CPU中的原始数据
    //    GL_STATIC_DRAW // 上传后通常不会频繁修改
    //);

    glBufferData(
        GL_ARRAY_BUFFER, // 操作哪个绑定目标上的缓冲区
        sizeof(rectangleVertices), // 数据有多少字节
        rectangleVertices, // 从哪里读取原始数据
        GL_STATIC_DRAW  // 预计如何使用这些数据
    );

    glVertexAttribPointer(
        0,                  // 对应着色器的 location = 0
        3,                  // X、Y、Z
        GL_FLOAT,
        GL_FALSE,
        vertexStride,
        nullptr             // 从每个顶点的第0字节开始
    );

    glEnableVertexAttribArray(0);

    // 顶点排列方式  告诉VAO如何读取VBO中的坐标
    glVertexAttribPointer(
        1, // 顶点属性位置 location = 1
        3, // 每个位置由x,y,z三个数构成
        GL_FLOAT, // 每个数都是float
        GL_FALSE, // 不进行归一化转换
        vertexStride, // 相邻两个顶点之间的字节距离
        reinterpret_cast<void*>(3 * sizeof(float))
    );

    glEnableVertexAttribArray(1);

    // 将索引上传到EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject); // 将刚才创建的EBO绑定到GL_ELEMENT_ARRAY_BUFFER上

    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        sizeof(rectangleIndices),
        rectangleIndices,
        GL_STATIC_DRAW
    );

    // 解除绑定
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // 输出对象编号
    std::cout << "VAO: " << vertexArrayObject << std::endl;
    std::cout << "VBO: " << vertexBufferObject << std::endl;
    std::cout << "EBO: " << elementBufferObject << std::endl;
    std::cout << "Rectangle vertex count: " << vertexCount << std::endl;
    std::cout << "Index count: " << indexCount << std::endl;
    std::cout << "Vertex count: " << vertexCount << std::endl;

    std::cout
        << "Triangle vertex count: "
        << vertexCount
        << std::endl;

    bool running = true;

    // 这两个变量位于CPU中，用来记录矩形当前移动了多少
    float objectOffsetX = 0.0f; //保存水平方向的偏移量，初始为 0
    float objectOffsetY = 0.0f; //保存垂直方向的偏移量，初始为 0

    constexpr float moveStep = 0.1f; //定义一个编译时常量 moveStep，值为 0.1f，用于表示每次执行移动更新时增加或减少多少

    while (running) {
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            else if (event.type == SDL_KEYDOWN && event.key.repeat == 0) { // event.key.repeat == 0：要求这次按键事件不是长按产生的自动重复事件
                switch (event.key.keysym.sym) { // event.key.keysym.sym 保存这次事件对应的具体键码
                case SDLK_w:
                    objectOffsetY += moveStep;
                    break;
                case SDLK_s:
                    objectOffsetY -= moveStep;
                    break;
                case SDLK_a:
                    objectOffsetX -= moveStep;
                    break;
                case SDLK_d:
                    objectOffsetX += moveStep;
                    break;
                case SDLK_ESCAPE:
                    running = false;
                    break;
                }
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

        glUniform2f(objectOffsetLocation, objectOffsetX, objectOffsetY); // 修改uniform变量 传入两个数 两个数都是float

        glBindVertexArray(vertexArrayObject); // 选择VAO 恢复VAO中保存的顶点读取规则 VAO会告诉OpenGL：位置数据来自哪个VBO、每个顶点包含3个float、属性位置为0

        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr); //每三个索引组成一个三角形, 读取6个索引, 索引类型对应GLuint, 从EBO开头读取

        // glDrawArrays(GL_TRIANGLES, 0, vertexCount); // 发出绘制命令：按三角形方式绘制、从第0个顶点开始、一共读取3个顶点

        glBindVertexArray(0); // 解除VAO绑定

        // 将OpenGL绘制结果显示到窗口
        SDL_GL_SwapWindow(window);
    }

    // 释放Shader Program
    glUseProgram(0);
    glDeleteProgram(shaderProgram);

    // 释放OpenGL对象
    glDeleteBuffers(1, &elementBufferObject);
    glDeleteBuffers(1, &vertexBufferObject);
    glDeleteVertexArrays(1, &vertexArrayObject);

    SDL_GL_DeleteContext(openGLContext);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}