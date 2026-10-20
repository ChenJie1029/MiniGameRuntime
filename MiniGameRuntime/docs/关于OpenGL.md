# 关于OpenGL

OpenGL的主要职责：

- 管理显卡绘图资源
- 接受顶点数据
- 执行着色器
- 将图形绘制到颜色缓冲区

## 当前程序中各部分职责

|       部分       |                职责                |
| :--------------: | :--------------------------------: |
|       SDL        | 创建窗口、读取事件、创建OpenGL环境 |
|  OpenGL Context  | 保存本次OpenGL绘图所需的状态和资源 |
|       GLAD       |    加载显卡驱动提供的OpenGL函数    |
| triangleVertices |         CPU中的三角形坐标          |
|       VBO        |      保存复制到显卡的顶点数据      |
|       VAO        |  保存“如何解释这些顶点数据的规则”  |
|      Shader      |           处理顶点和像素           |

流程：

CPU中的三角形坐标经过glBufferData复制得到VBO顶点原始数据，同时VAO也就是顶点格式说明，这两个一个提供数据，一个提供操作说明，两个通过OpenGL绘图再输出到窗口画面

## 代码部分

### 文件开头

~~~c++
#include <glad/gl.h>
~~~

引入GLAD提供的OpenGL声明，使程序可以使用：

>glViewport();  设置OpenGL画面显示区域
>glGenBuffers();  申请缓冲区编号
>glBufferData();  创建缓冲区存储并上传数据
>glClear();  清空当前帧的缓冲区

~~~c++
#include <SDL.h>
~~~

引入SDL，使程序能创建窗口、处理输入和创建OpenGL上下文

~~~c++
constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGHT = 600;
~~~

定义窗口尺寸

### 程序入口

~~~c++
int main(int argc, char* argv[])
~~~

- argc：命令行参数数量
- argv：命令行参数内容

当前没有使用它们，但SDL项目经常保留这种标准入口形式

### 初始化SDL

~~~c++
if(SDL_Init(SDL_INIT_VIDEO) != 0)
~~~

`SDL_Init(SDL_INIT_VIDEO)` 初始化SDL的视频系统

返回0成功，返回非0失败

~~~c++
SDL_GetError()
~~~

获取SDL记录的最近一次错误说明

OpenGL 3.3核心模式

~~~c++
SDL_GL_SetAttribute(
    SDL_GL_CONTEXT_MAJOR_VERSION,
    3
);
~~~

要求OpenGL主版本号为3

~~~c++
SDL_GL_SetAttribute(
    SDL_GL_CONTEXT_MINOR_VERSION,
    3
);
~~~

要求次版本号为3

~~~c++
SDL_GL_SetAttribute(
    SDL_GL_CONTEXT_PROFILE_MASK,
    SDL_GL_CONTEXT_PROFILE_CORE
);
~~~

要求使用核心模式，核心模式意味着使用现代OpenGL方式：

- VAO
- VBO
- 着色器

~~~c++
SDL_GL_SetAttribute(
    SDL_GL_DOUBLEBUFFER,
    1
);
~~~

启用双缓冲，双缓冲可以理解为两张画布

>前缓冲：当前显示在屏幕上的画面
>
>后缓冲：程序正在绘制的下一帧画面

下一帧画完后交换两张画布，可以避免用户看到绘制到一半的画面

### 创建窗口

~~~c++
    SDL_Window* window = SDL_CreateWindow( // 创建SDL窗口，并将返回的窗口地址保存到:window 
                                           // SDL_Window* 是指向SDL窗口对象的指针
        "MiniGameRuntime - OpenGL", // 窗口标题
        SDL_WINDOWPOS_CENTERED, // 让窗口在屏幕中央出现
        SDL_WINDOWPOS_CENTERED, //
        WINDOW_WIDTH, // 窗口宽度为800
        WINDOW_HEIGHT, // 高度为600
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN // 两个窗口选项
        // SDL_WINDOW_OPENGL：该窗口用于OpenGL绘图
        // SDL_WINDOW_SHOWN：创建后立即显示窗口
    );
~~~

~~~c++
    if (window == nullptr) { // 如果窗口创建失败，SDL返回空指针
        std::cout
            << "Window creation failed: "
            << SDL_GetError()
            << std::endl;

        SDL_Quit();
        return 1; // 此时程序输出错误、关闭SDL并结束
    }
~~~

### 创建OpenGL上下文

~~~c++
SDL_GLContext openGLContext = SDL_GL_CreateContext(window); // 当前程序和显卡之间的一次绘图工作环境
~~~

它保存了大量状态，例如：

- 当前绑定的VAO
- 当前绑定的VBO
- 当前使用的着色器
- 清屏颜色
- 已创建的OpenGL资源

没有上下文就不能调用OpenGL绘图函数

~~~c++
    if (openGLContext == nullptr) { // 检查上下文是否创建失败
        std::cout
            << "OpenGL context creation failed: "
            << SDL_GetError()
            << std::endl;

        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1; // 如果创建失败，按照创建顺序的反方向释放
    }
~~~

### 开启垂直同步

~~~c++
SDL_GL_SetSwapInterval(1) // 参数1表示启用垂直同步
~~~

它会让画面交换尽量与显示器刷新节奏同步，例如60Hz显示器通常限制到大约60FPS

如果启用失败，只输出警告，不需要结束程序，因为程序仍然可以运行

### GLAD加载OpenGL函数

```c++
int openGLVersion = gladLoadGL(
    reinterpret_cast<GLADloadfunc>(
        SDL_GL_GetProcAddress
        )
);

// SDL_GL_GetProcAddress // 负责查询函数地址
// reinterpret_cast<GLADloadfunc> // 将SDL提供的函数形式转换成GLAD要求的函数形式
```

让GLAD通过SDL向显卡驱动查询OpenGL函数的真实地址

Windows本身并不会直接提供所有现代OpenGL函数地址，因此GLAD需要动态查询

```c++
if (openGLVersion == 0) { // 如果返回0，表示GLAD加载失败
    std::cout
        << "Failed to initialize GLAD!"
        << std::endl;
    // 如果加载失败，必须释放 
    SDL_GL_DeleteContext(openGLContext); // OpenGL上下文
    SDL_DestroyWindow(window); // SDL窗口
    SDL_Quit(); // SDL系统

    return 1;
}
```

~~~c++
    std::cout
        << "Loaded OpenGL "
        << GLAD_VERSION_MAJOR(openGLVersion) // 从GLAD返回值中取得实际加载成功的OpenGL版本
        << "."
        << GLAD_VERSION_MINOR(openGLVersion)
        << std::endl;
~~~

### 设置窗口

~~~c++
    glViewport( // 告诉 OpenGL 最终图像应该输出到窗口中的哪块矩形区域
        0, // 左下角X = 0
        0, // 左下角Y = 0
        WINDOW_WIDTH, // 宽度 = 800
        WINDOW_HEIGHT // 高度 = 600
    );
~~~

### CPU中的三角形数据

~~~c++
    constexpr float triangleVertices[] = { // 这个数组仍然位于CPU内存
        // X      Y      Z
        -0.5f, -0.5f,  0.0f,  // 左下
         0.5f, -0.5f,  0.0f,  // 右下
         0.0f,  0.5f,  0.0f   // 上方
    };
~~~

| 顶点 | X    | Y    | Z    |
| ---- | ---- | ---- | ---- |
| 左下 | -0.5 | -0.5 | 0.0  |
| 右下 | 0.5  | -0.5 | 0.0  |
| 上方 | 0.0  | 0.5  | 0.0  |

后面的VAO配置才会告诉OpenGL：每三个float应当看作一个顶点位置

~~~c++
constexpr int valuesPerVertex = 3;
~~~

表示每个顶点包含三个值：x、y、z

~~~c++
    constexpr GLsizei vertexCount = // 计算顶点数量
        static_cast<GLsizei>( // GLsizei是OpenGL用于表示数量的一种整数类型
            sizeof(triangleVertices) /
            sizeof(triangleVertices[0]) /
            valuesPerVertex
            );

// sizeof(triangleVertices) // 得到整个数组的字节数。通常一个float是4字节  9 × 4 = 36字节
// sizeof(triangleVertices[0]) // 得到一个 float 的大小，也就是4字节
~~~

### VAO和VBO编号

~~~c++
    // GLuint：OpenGL 用来表示对象编号的类型  GLuint是OpenGL常用的无符号整数类型  
    GLuint vertexArrayObject = 0; // VAO:记录“顶点数据应该怎样读取”的配置
    GLuint vertexBufferObject = 0; // VBO:显卡中的一块缓冲区，真正保存顶点数据 

// 这些变量不是对象本身，也不是指针，而是OpenGL资源的编号
// vertexArrayObject = VAO的取件号码
// vertexBufferObject = VBO的取件号码
~~~

~~~c++
    glGenVertexArrays(1, &vertexArrayObject); 
    // 向OpenGL申请一个VAO编号  生成一个VAO 将生成的编号写入vertexArrayObject这个变量
    glGenBuffers(1, &vertexBufferObject);
    // 向OpenGL申请一个缓冲区编号，这里将作为VBO使用
~~~

### 关于绑定

OpenGL是一套状态机风格的API

很多函数不会直接接收操作对象，例如：

~~~c++
glBindVertexArray(vertexArrayObject);
~~~

里面没有传入 `vertexBufferObject`

OpenGL采用的是：先选中某个对象，再对当前选中的对象执行操作

~~~c++
    // 选中vertexBufferObject → 向当前选中的缓冲区上传数据
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject); 
~~~

### 绑定VAO

~~~c++
glBindVertexArray(vertexArrayObject);
~~~

将刚才创建的VAO设置为当前VAO，后面的顶点读取配置会被记录进这个VAO

### 绑定VBO

~~~c++
glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
~~~

把 `vertexBufferObject` 绑定到顶点数组缓冲区位置

> 接下来对GL_ARRAY_BUFFER的操作
> 都作用于vertexBufferObject

### 上传顶点数据

~~~c++
    glBufferData(
        GL_ARRAY_BUFFER, // 上传的是顶点数据
        sizeof(triangleVertices), // 数据总字节数
        triangleVertices, // CPU中的原始数据
        GL_STATIC_DRAW // 上传后通常不会频繁修改
    );
~~~

把CPU数组复制到VBO

```
CPU中的triangleVertices
          ↓ 复制
VBO中的显卡数据
```

即使后面CPU数组消失，VBO中的数据仍然存在，直到删除VBO。

严格来说，数据的实际存储位置由显卡驱动决定，但当前阶段可以把VBO理解成显卡中的缓冲区。

### 描述顶点格式

~~~c++
    // 顶点排列方式
    glVertexAttribPointer(
        0, // 顶点属性位置 location = 0  着色器会写layout(location = 0) in vec3 position;
        valuesPerVertex, // 每个位置由x,y,z三个数构成
        GL_FLOAT, // 每个分量的数据类型都是float
        GL_FALSE, // 不进行归一化转换
        valuesPerVertex * sizeof(float), // 相邻两个顶点之间的字节距离 3 × 4 = 12字节
        nullptr // 第一个属性从缓冲区第0字节开始
    );
~~~

告诉OpenGL如何解释VBO中的数据

| 字节偏移 | 数据     |
| -------- | -------- |
| 0        | 顶点0的X |
| 4        | 顶点0的Y |
| 8        | 顶点0的Z |
| 12       | 顶点1的X |
| 16       | 顶点1的Y |
| 20       | 顶点1的Z |

### 启用属性0

~~~c++
glEnableVertexAttribArray(0);
~~~

前面的 `glVertexAttribPointer()` 只是填写配置，这一行才真正启用位置编号0

> 属性0已经配置完成 → 现在允许OpenGL读取属性0

### 解除绑定

~~~c++
glBindBuffer(GL_ARRAY_BUFFER, 0); // 取消当前VBO绑定
glBindVertexArray(0); //取消当前VAO绑定
~~~

这里的 `0` 表示不再选中任何自己创建的对象，解除绑定不会删除资源，也不会清空数据。

VAO已经记录了：

> 属性0
> → 来自哪个VBO
> → 每次读取3个float
> → 每个顶点相隔12字节
> → 从第0字节开始

### 输出编号

~~~c++
    std::cout << "VAO: " << vertexArrayObject << std::endl;
    std::cout << "VBO: " << vertexBufferObject << std::endl;
    std::cout << "Vertex count: " << vertexCount << std::endl;
~~~

用于确认对象生成成功

> VAO: 1
> VBO: 1
> Triangle vertex count: 3

### 程序循环

~~~c++
    bool running = true; // 控制程序是否继续运行

    while (running) { // 只要running为真，程序就持续执行每一帧
        SDL_Event event; // 创建一个SDL事件变量

        while (SDL_PollEvent(&event)) { // 不断从SDL事件队列中取出事件
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
        SDL_GL_SwapWindow(window); // 交换前缓冲和后缓冲
    }
~~~

### 删除OpenGL对象

~~~c++
    glDeleteBuffers(1, &vertexBufferObject); // 删除VBO，释放其缓冲区资源
    glDeleteVertexArrays(1, &vertexArrayObject); // 删除VAO
~~~

### 清理SDL和OpenGL

~~~c++
    SDL_GL_DeleteContext(openGLContext); // 删除OpenGL上下文  执行这一步以后就不应该继续调用普通OpenGL绘图函数
    SDL_DestroyWindow(window); // 销毁SDL窗口
    SDL_Quit(); // 关闭SDL系统
~~~

