#include <ren/texture.hpp>
#include <glad/glad.h>
#include <stb_image/stb_image.h>

using namespace ren;

TextureFormat TextureFormat::NearestRGBA(int mipmaps) {
	return {false, GL_RGBA8, GL_RGBA, mipmaps};
}

TextureFormat TextureFormat::LinearRGBA(int mipmaps) {
	return {true, GL_RGBA8, GL_RGBA, mipmaps};
}

TextureFormat TextureFormat::NearestFont() {
	return {false, GL_RED, GL_RED, 0};
}

TextureFormat TextureFormat::LinearFont() {
	return {true, GL_RED, GL_RED, 0};
}

Texture::Texture() : width(0), height(0), id(0) {}

void Texture::create(int width, int height, void* data, TextureFormat format) {
	this->width = width;
	this->height = height;

	// Generate and bind
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

	// Horizontal and vertical repeating and filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	if (format.mipmaps != 0) {
		glTexParameteri(
			GL_TEXTURE_2D,
			GL_TEXTURE_MIN_FILTER,
			format.filtered ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_NEAREST);
	} else {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, format.filtered ? GL_LINEAR : GL_NEAREST);
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, format.filtered ? GL_LINEAR : GL_NEAREST);

	// Load texture into GPU
	glTexImage2D(
		GL_TEXTURE_2D,        // Target
		format.mipmaps,       // Mipmap level for manual mipmaps
		format.textureFormat, // Texture format
		width,                // Texture width
		height,               // Texture height
		0,                    // 0 is some "legacy stuff"
		format.sourceFormat,  // Source format
		GL_UNSIGNED_BYTE,     // Source datatype
		data);                // Source data
	if (format.mipmaps != 0) {
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::create(std::string_view fileName, bool filtered) {
	int width, height, num_channels;
	void* data = stbi_load(fileName.data(), &width, &height, &num_channels, 0);
	this->width = width;
	this->height = height;

	if (filtered) {
		create(width, height, data, TextureFormat::LinearRGBA(4));
	} else {
		create(width, height, data, TextureFormat::NearestRGBA(4));
	}
	stbi_image_free((stbi_uc*)data);
}

void Texture::create() {
	constexpr uchar whitePixel[4] = {0xff, 0xff, 0xff, 0xff};
	create(1, 1, (void*)whitePixel, TextureFormat::LinearRGBA());
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
