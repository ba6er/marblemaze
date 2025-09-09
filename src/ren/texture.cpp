#include <ren/texture.hpp>
#include <glad/glad.h>
#include <stb_image/stb_image.h>

using namespace ren;

Texture::Texture() : width(0), height(0), id(0) {}

void Texture::create(int width, int height, void* data, bool filtered) {
	this->width = width;
	this->height = height;

	// Generate and bind
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

	// Horizontal and vertical repeating and filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtered ? GL_LINEAR : GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtered ? GL_LINEAR : GL_NEAREST);

	// Load texture into GPU
	glTexImage2D(
		GL_TEXTURE_2D,    // Target
		0,                // Mipmap level for manual mipmaps
		GL_RGBA8,         // Texture format
		width,            // Texture width
		height,           // Texture height
		0,                // 0 is some "legacy stuff"
		GL_RGBA,          // Source format
		GL_UNSIGNED_BYTE, // Source datatype
		data);            // Source data

	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::create(std::string_view fileName, bool filtered) {
	int width, height, num_channels;
	void* data = stbi_load(fileName.data(), &width, &height, &num_channels, 0);
	this->width = width;
	this->height = height;

	create(width, height, data, filtered);
	stbi_image_free((stbi_uc*)data);
}

void Texture::create() {
	constexpr uchar whitePixel[4] = {0xff, 0xff, 0xff, 0xff};
	create(1, 1, (void*)whitePixel, false);
}

void Texture::destroy() {
	glDeleteTextures(1, &id);
}

void Texture::use(int index) const {
	glActiveTexture(GL_TEXTURE0 + index);
	glBindTexture(GL_TEXTURE_2D, id);
}

int Texture::getWidth() const {
	return width;
}

int Texture::getHeight() const {
	return height;
}

uint Texture::getId() const {
	return id;
}
