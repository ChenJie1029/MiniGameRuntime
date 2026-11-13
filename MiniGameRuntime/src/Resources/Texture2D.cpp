#define STB_IMAGE_IMPLEMENTATION

#include "ThirdParty/stb_image.h"
#include "Resources/Texture2D.h"

#include <iostream>

bool Texture2D::LoadFromFile(const std::string& path) { // 传入图片路径
	int width = 0;
	int height = 0;
	int channels = 0;

	unsigned char* pixels = stbi_load(path.c_str(), &width, &height, &channels, STBI_rgb_alpha);

	if (pixels == nullptr) {
		std::cerr << "Failed to load texture: " << path << '\n';
		std::cerr << "Reason: " << stbi_failure_reason() << '\n';
		return false;
	}

	glGenTextures(1, &m_textureId); // 向OpenGL申请一个纹理编号  &m_textureId是成员变量地址  OpenGL 把申请到的整数写进去
	glActiveTexture(GL_TEXTURE0); // 选择第0号管理单元
	glBindTexture(GL_TEXTURE_2D, m_textureId); // 把这个编号对应的纹理绑定到当前单元的二维纹理目标。之后对 GL_TEXTURE_2D 设置参数、上传图片，都会作用到这张纹理

	// glTexParameteri() 第一个参数选中已绑定的二维纹理，第二个参数说明要设置哪项规则，第三个参数给出规则的值
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // GL_TEXTURE_WRAP_S  	横向纹理坐标超出范围时，`GL_CLAMP_TO_EDGE` 让采样停在边缘
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // GL_TEXTURE_WRAP_T   纵向纹理坐标超出范围时，同样取边缘颜色
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // GL_TEXTURE_MIN_FILTER   图片缩小时，`GL_LINEAR_MIPMAP_LINEAR` 在相邻两级较小的纹理中取样并混合
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  // GL_TEXTURE_MAG_FILTER  图片放大时，`GL_LINEAR` 混合附近像素的颜色

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	// 操作当前绑定的二维纹理,
	// 上传到第0级也就是原始尺寸的那层,
	// 指定OpenGL内部保存的颜色分量,
	// 图片宽高单位是像素,
	// 边框宽度当前OpenGL Core接口要求填0,
	// 告诉OpenGL传进来的像素按R、G、B、A 排列,
	// 每个颜色分量占一个无符号字节,
	// 从这个 CPU 内存地址读取像素

	glGenerateMipmap(GL_TEXTURE_2D); // 根据第0级生成更小的纹理层
	stbi_image_free(pixels); // 释放解码时分配的CPU缓冲区

	return true;
}

GLuint Texture2D::GetId() const {  // 把编号按值返回。main()把编号交给DrawEntity，绘制代码就可以用它绑定那张纹理。每帧传来传去的是一个整数。
	return m_textureId;
}

void Texture2D::Destroy() { 
	if (m_textureId == 0) {
		return;
	}

	glDeleteTextures(1, &m_textureId);
	m_textureId = 0;
}
// Destroy()先检查编号是否为0。有纹理时glDeleteTextures(1, &m_textureId)从这个地址读取1个编号并删除对应的纹理，下一行再把成员设回0。
// 对比前面的glGenTextures，同样传入 &m_textureId，创建时OpenGL写入编号，删除时OpenGL读取编号。主函数在SDL_GL_DeleteContext 之前调用 Destroy()，这时 OpenGL 上下文仍然可用。