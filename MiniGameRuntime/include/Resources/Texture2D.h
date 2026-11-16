#pragma once

#include <string>
#include <glad/gl.h>

class Texture2D {
public:
	Texture2D() = default; // 让编译器生成默认构造函数  写Texture2D texture;时，成员m_textureId从0开始
	~Texture2D(); // 声明析构函数  类名前面的 ~ 标识析构函数  局部对象离开作用域时C++自动调用它

	Texture2D(const Texture2D&) = delete; // 禁止拷贝构造
	Texture2D& operator = (const Texture2D&) = delete; // 禁止拷贝赋值

	bool LoadFromFile(const std::string& path);
	GLuint GetId() const;

private:
	GLuint m_textureId = 0;
};