#include <iostream>
#include <cmath>

#include <glad/gl.h>
#include <SDL.h>

constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGHT = 600;

float objectOffsetX = 0.0f;
float objectOffsetY = 0.0f;
float objectRotation = 0.0f;

constexpr float objectScaleX = 0.8f;
constexpr float objectScaleY = 0.5f;

constexpr float moveSpeed = 1.0f;
constexpr float rotationSpeed = 1.0f;

struct Mat4 {
    float values[16];
};

Mat4 CreateModelMatrix(float positionX, float positionY, float rotationRadians, float scaleX, float scaleY)
{ // 控制X平移  控制Y平移  控制旋转  控制X缩放  控制Y缩放
    float cosine = std::cos(rotationRadians); // 进一步控制旋转
    float sine = std::sin(rotationRadians);

    Mat4 matrix{
        {
            cosine * scaleX,
            sine * scaleX,
            0.0f,
            0.0f,

            -sine * scaleY,
            cosine * scaleY,
            0.0f,
            0.0f,

            0.0f,
            0.0f,
            1.0f,
            0.0f,

            positionX,
            positionY,
            0.0f,
            1.0f
        }
    };

    return matrix;
}

int main(int argc, char* argv[])
{
    // ----------添加顶点着色器----------
const char* vertexShaderSource = R"glsl(
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec2 textureCoordinate;

uniform mat4 modelMatrix;

out vec3 vertexColor;
out vec2 textureUV;

void main()
{
    gl_Position = modelMatrix * vec4(position, 1.0);    

    vertexColor = color;
    textureUV = textureCoordinate;
}
)glsl";
    // #version 330 core   使用GLSL 3.30核心模式，与OpenGL 3.3对应
    // layout(location = 0) in vec3 position;   从VAO的属性位置0读取三个数 x、y、z
    // gl_Position = vec4(position, 1.0);   将输入位置设置为顶点最终位置  gl_Position 要求四个分量x、y、z、w  所以将vec3转换成vec4(position, 1.0)

    // ----------添加片段着色器----------
const char* fragmentShaderSource = R"glsl(
#version 330 core

in vec3 vertexColor;
in vec2 textureUV;

uniform sampler2D objectTexture;

out vec4 fragmentColor;

void main()
{
    fragmentColor = texture(objectTexture, textureUV);
}
)glsl";
    // out vec4 fragmentColor;   声明最终输出颜色
    // vec4(1.0, 0.5, 0.2, 1.0)  红 = 1.0，绿 = 0.5，蓝 = 0.2，透明度 = 1.0
    // R"glsl(... )glsl" 是C++原始字符串，使我们可以直接在C++中保存多行着色器代码
    // fragmentColor = texture(objectTexture, textureUV);  使用当前像素的UV坐标，从 objectTexture 中读取颜色    objectTexture指定从哪张纹理取样   textureUV指定取样位置

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

    // glGetUniformLocation在指定ShaderProgram中查找名为modelMatrix的变量，并返回它的位置编号是modelMatrixLocation类型
    GLint modelMatrixLocation = glGetUniformLocation(shaderProgram, "modelMatrix");  // 存在于顶点着色器中 uniform mat4 modelMatrix;
    // 检查是否找到
    if (modelMatrixLocation == -1) {
        std::cout << "Failed to find modelMatrix uniform!" << std::endl;
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
        // 位置：X      Y      Z    颜色：R   G   B   // UV
        -0.5f, -0.35f, 0.0f, 1.0, 0.0f, 0.0f, 0.0f, 0.0f,  // 0: 左下  红
        0.5f, -0.35f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // 1: 右下  绿
        0.5f, 0.35f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,   // 2: 右上  蓝
        -0.5f, 0.35f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f   // 3: 左上  黄
    };

    const unsigned char texturePixels[] = { // 这是四个RGBA像素  用四个像素，制作并上传一张 2×2 的彩色纹理，同时规定它放大、缩小时如何取颜色
        255,   0,   0, 255,     0, 255,   0, 255,
          0,   0, 255, 255,   255, 255,   0, 255
    };

    GLuint texture = 0; // 准备一个无符号整数变量texture，用于保存纹理编号
    glGenTextures(1, &texture); // 申请一个纹理编号，并写入texture
    glBindTexture(GL_TEXTURE_2D, texture); // 把这个编号对应的纹理绑定为当前操作的二维纹理。之后针对 GL_TEXTURE_2D 的参数设置和图片上传，就作用于它

    // 设置纹理的取样规则
    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_MIN_FILTER, // 纹理显示得比较小时，怎样取颜色
        GL_NEAREST // 直接采用距离取样位置最近的纹理像素的颜色
    );

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_MAG_FILTER, // 纹理显示得比较大时，怎样取颜色
        GL_NEAREST
    );

    // 创建一张宽2、高2、每个像素使用RGBA格式的二维纹理，并上传texturePixels 定义纹理图片的尺寸、存储格式，并上传像素数据
    glTexImage2D(
        GL_TEXTURE_2D, // 操作当前绑定的二维纹理
        0, // 设置第 0 层，也就是基础图像层
        GL_RGBA, // 指定纹理内部保存红、绿、蓝、Alpha 分量
        2, // 图片宽度为 2 个像素
        2, // 图片高度为 2 个像素
        0, // 保留的边框参数，必须为 0
        GL_RGBA, // 原始数据按 R、G、B、A 的顺序排列
        GL_UNSIGNED_BYTE, // 原始数据的每个分量是一个无符号字节
        texturePixels // 从这个数组中读取像素数据
    );

    // 索引数组 索引中的数字表示 使用的是第几个顶点
    constexpr GLuint rectangleIndices[] = {
        0, 1, 2, // 第一个三角形
        0, 2, 3  // 第二个三角形
    };

    // 计算索引数量 6
    constexpr GLsizei indexCount = static_cast<GLsizei>(sizeof(rectangleIndices) / sizeof(rectangleIndices[0]));

    constexpr int valuesPerVertex = 8;

    constexpr GLsizei vertexStride = valuesPerVertex * sizeof(float); // 从当前顶点开头到移动到下一个顶点开头需要跨过多少字节

    constexpr GLsizei vertexCount =
        static_cast<GLsizei>(
            sizeof(rectangleVertices) /
            sizeof(rectangleVertices[0]) /
            valuesPerVertex
        );

    GLint textureLocation = glGetUniformLocation(shaderProgram, "objectTexture");

    glUseProgram(shaderProgram);
    glUniform1i(textureLocation, 0);

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

    glVertexAttribPointer(
        2,
        2,
        GL_FLOAT,
        GL_FALSE,
        vertexStride,
        reinterpret_cast<void*>(6 * sizeof(float))
    );
    glEnableVertexAttribArray(2);

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

    // constexpr float moveStep = 0.1f; //定义一个编译时常量 moveStep，值为 0.1f，用于表示每次执行移动更新时增加或减少多少
    constexpr float moveSpeed = 0.8f;

    // 添加计时器
    const Uint64 performanceFrequency = SDL_GetPerformanceFrequency(); // 计时器每秒计数多少次
    Uint64 previousCounter = SDL_GetPerformanceCounter(); // 取计数器当前的读数，保存到previousCounter，作为之后计算时间差的起点

    while (running) {
        Uint64 currentCounter = SDL_GetPerformanceCounter(); // 取计数器当前的度数，保存到currentCounter，用于计算和previousCounter的差值
        float deltaTime = static_cast<float>(currentCounter - previousCounter) / static_cast<float>(performanceFrequency); // 计数差值 / 计数频率 = 具体经过时间

        previousCounter = currentCounter;

        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                running = false;
            }
        }

        if (!running) {
            break;
        }

        const Uint8* keyboardState = SDL_GetKeyboardState(nullptr); // 第一行是取得键盘按键状态表的地址 1：这个键正被按住 0：这个键没有被按住

        // 后两行是准备保存当前想移动的方向，初始都为0
        float directionX = 0.0f;
        float directionY = 0.0f;

        if (keyboardState[SDL_SCANCODE_W]) {
            directionY += 1.0f;
        }

        if (keyboardState[SDL_SCANCODE_S]) {
            directionY -= 1.0f;
        }

        if (keyboardState[SDL_SCANCODE_A]) {
            directionX -= 1.0f;
        }

        if (keyboardState[SDL_SCANCODE_D]) {
            directionX += 1.0f;
        }

        // 加入归一化操作确保斜向速度和正向速度保持一致
        float directionLength = std::sqrt(directionX * directionX + directionY * directionY);
        if (directionLength > 0.0f) {
            directionX /= directionLength;
            directionY /= directionLength;
        }

        objectOffsetX += directionX * moveSpeed * deltaTime;
        objectOffsetY += directionY * moveSpeed * deltaTime;

        objectRotation += rotationSpeed * deltaTime; // 物体持续旋转

        Mat4 modelMatrix = CreateModelMatrix(objectOffsetX, objectOffsetY, objectRotation, objectScaleX, objectScaleY);

        // 指定OpenGL清屏颜色 OpenGL颜色范围是：0.0 ～ 1.0  红、绿、蓝、透明度
        glClearColor(
            0.12f,
            0.12f,
            0.18f,
            1.0f
        );

        // 清除颜色缓冲区
        glClear(GL_COLOR_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        glUseProgram(shaderProgram); // 选择着色器程序  告诉OpenGL接下来的绘制使用这个着色器程序

        // glUniform2f(objectOffsetLocation, objectOffsetX, objectOffsetY); // 修改uniform变量 传入两个数 两个数都是float
        glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, modelMatrix.values); // Shader中modelMatrix的位置  发送一个矩阵  不转置矩阵  CPU端保存矩阵数据的数组

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
    glDeleteTextures(1, &texture);

    SDL_GL_DeleteContext(openGLContext);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}