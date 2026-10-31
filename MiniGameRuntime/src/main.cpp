#include <iostream>
#include <cmath>
#include <string>

#include <glad/gl.h>
#include <SDL.h>

#define STB_IMAGE_IMPLEMENTATION
#include "ThirdParty/stb_image.h"

#include "Math/Vec2.h"

constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGHT = 600;

struct Mat4 {
    float values[16];
};

struct Sprite {
    Vec2 position;
    Vec2 scale;
    float rotationRadians;
    GLuint textureId;
};

struct Camera2D {
    Vec2 position;
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
uniform mat4 viewMatrix;

out vec3 vertexColor;
out vec2 textureUV;

void main()
{
    gl_Position = viewMatrix * modelMatrix * vec4(position, 1.0);

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

    GLint viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix"); // 查找viewMatrix的位置
    if (viewMatrixLocation == -1) {
        std::cout
            << "Failed to find viewMatrix uniform!"
            << std::endl;
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

    glEnable(GL_BLEND); // 启用颜色混合，允许当前片元的颜色与画面中已有的背景颜色混合

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // 这句规定混合比例(当前片元颜色乘以自己的Alpha, 背景颜色乘以 1−当前片元的Alpha)    最终颜色 = 当前颜色×Alpha + 背景颜色×(1 − Alpha)

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

    const std::string texturePath = std::string(MGR_ASSET_DIR) + "test.png"; // 拼接图片路径

    stbi_set_flip_vertically_on_load(true); // 设置加载选项，接下来读取图片时把像素行上下翻转，让返回数据从图片底部开始排列，方便配合前面使用的 OpenGL 纹理坐标约定

    // 准备三个变量，供加载函数填写结果
    int textureWidth = 0; // 图片宽度
    int textureHeight = 0; // 图片高度
    int textureChannels = 0; // 原始图片通道数

    unsigned char* texturePixels = stbi_load( // stbi_load负责读取并解码图片  函数返回像素数据的起始地址，保存到 texturePixels
        texturePath.c_str(), // 提供路径字符串的 const char* 地址
        &textureWidth,
        &textureHeight,
        &textureChannels,
        STBI_rgb_alpha // 要求返回的像素统一使用RGBA四通道格式
    );

    if (texturePixels == nullptr) {
        std::cout
            << "Failed to load texture: "
            << texturePath
            << std::endl;

        std::cout
            << "Reason: "
            << stbi_failure_reason()
            << std::endl;

        glDeleteProgram(shaderProgram); // 	请求删除之前创建的着色器程序，释放其资源

        SDL_GL_DeleteContext(openGLContext);
        SDL_DestroyWindow(window);
        SDL_Quit();

        return 1;
    }

    GLuint texture = 0; // 准备一个无符号整数变量texture，用于保存纹理编号

    glGenTextures(1, &texture); // 申请一个纹理编号，并写入texture

    glActiveTexture(GL_TEXTURE0); // 选择第0号纹理单元。片段着色器中的 objectTexture之后也会使用0号纹理单元

    glBindTexture(GL_TEXTURE_2D, texture); // 把这个编号对应的纹理绑定为当前操作的二维纹理。之后针对 GL_TEXTURE_2D 的参数设置和图片上传，就作用于它

    // 设置纹理的取样规则  设置纹理的边界处理方式：当 UV 超出图片范围时，使用最近的边缘颜色
    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_WRAP_S,
        GL_CLAMP_TO_EDGE
    );

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_WRAP_T,
        GL_CLAMP_TO_EDGE
    );

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_MIN_FILTER, // 纹理显示得比较小时，怎样取颜色
        GL_LINEAR_MIPMAP_LINEAR 
    );

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_MAG_FILTER, // 纹理显示得比较大时，怎样取颜色
        GL_LINEAR
    );

    // 创建一张宽2、高2、每个像素使用RGBA格式的二维纹理，并上传texturePixels 定义纹理图片的尺寸、存储格式，并上传像素数据
    //glTexImage2D(
    //    GL_TEXTURE_2D, // 操作当前绑定的二维纹理
    //    0, // 设置第 0 层，也就是基础图像层
    //    GL_RGBA, // 指定纹理内部保存红、绿、蓝、Alpha 分量
    //    2, // 图片宽度为 2 个像素
    //    2, // 图片高度为 2 个像素
    //    0, // 保留的边框参数，必须为 0
    //    GL_RGBA, // 原始数据按 R、G、B、A 的顺序排列
    //    GL_UNSIGNED_BYTE, // 原始数据的每个分量是一个无符号字节
    //    texturePixels // 从这个数组中读取像素数据
    //);

    // 上传图片
    glTexImage2D(
        GL_TEXTURE_2D, // 操作当前绑定的二维纹理
        0, // 上传到第 0 层：原始图像层
        GL_RGBA, // 纹理内部保存 RGBA 分量
        textureWidth, // 图片实际宽度
        textureHeight, // 图片实际高度
        0, // 边框参数，必须为 0
        GL_RGBA, // 传入像素按 R、G、B、A 排列
        GL_UNSIGNED_BYTE, // 每个分量是一个无符号字节
        texturePixels // 从这个地址读取像素数据
    );

    glGenerateMipmap(GL_TEXTURE_2D); // 为当前二维纹理生成 mipmap，也就是同一张图片的一组逐级缩小版本

    stbi_image_free(texturePixels); // 释放 stbi_load() 为图片分配的 CPU 内存

    // 这两个变量位于CPU中，用来记录矩形当前移动了多少
    Sprite playerSprite{
        Vec2{0.0f, 0.0f}, //保存水平方向的偏移量，初始为 0   保存垂直方向的偏移量，初始为 0
        Vec2{0.8f, 0.5f},
        0.0f,
        texture
    };

    // constexpr float moveStep = 0.1f; //定义一个编译时常量 moveStep，值为 0.1f，用于表示每次执行移动更新时增加或减少多少
    constexpr float moveSpeed = 0.8f;
    constexpr float rotationSpeed = 1.0f;

    // 创建Camera
    Camera2D camera{
        Vec2{0.0f, 0.0f}
    };

    constexpr float cameraSpeed = 0.6f;

    // 索引数组 索引中的数字表示 使用的是第几个顶点
    constexpr GLuint rectangleIndices[] = {
        0, 1, 2, // 第一个三角形
        0, 2, 3  // 第二个三角形
    };

    // 计算索引数量 8
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

        playerSprite.position.x += directionX * moveSpeed * deltaTime;
        playerSprite.position.y += directionY * moveSpeed * deltaTime;

        playerSprite.rotationRadians += rotationSpeed * deltaTime; // 物体持续旋转
        
        Mat4 modelMatrix = CreateModelMatrix(playerSprite.position.x, playerSprite.position.y, playerSprite.rotationRadians, playerSprite.scale.x, playerSprite.scale.y);

        Mat4 viewMatrix = CreateModelMatrix(-camera.position.x, -camera.position.y, 0.0f, 1.0f, 1.0f);

        // 移动摄像头视角
        if (keyboardState[SDL_SCANCODE_UP]) {
            camera.position.y += cameraSpeed * deltaTime;
        }

        if (keyboardState[SDL_SCANCODE_DOWN]) {
            camera.position.y -= cameraSpeed * deltaTime;
        }

        if (keyboardState[SDL_SCANCODE_LEFT]) {
            camera.position.x -= cameraSpeed * deltaTime;
        }

        if (keyboardState[SDL_SCANCODE_RIGHT]) {
            camera.position.x += cameraSpeed * deltaTime;
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

        glActiveTexture(GL_TEXTURE0);

        glBindTexture(GL_TEXTURE_2D, playerSprite.textureId);

        glUseProgram(shaderProgram); // 选择着色器程序  告诉OpenGL接下来的绘制使用这个着色器程序

        // glUniform2f(objectOffsetLocation, objectOffsetX, objectOffsetY); // 修改uniform变量 传入两个数 两个数都是float
        glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, modelMatrix.values); // Shader中modelMatrix的位置  发送一个矩阵  不转置矩阵  CPU端保存矩阵数据的数组

        glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, viewMatrix.values); // viewMatrixLocation 需要修改的矩阵

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