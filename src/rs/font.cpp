#include <rs/font.hpp>
#include <rs/resourcemanager.hpp>
#include <stb_truetype/stb_truetype.h>
#include <cmath>
#include <fstream>

using namespace rs;

Font::Font() : size(0), texture(nullptr) {
	for (int i = 0; i < NumGlyphs; i++) {
		glyphs[i] = {0, 0, {0, 0, 0, 0}};
	}
}

void Font::create(Texture& texture, std::string_view fileName, int size, bool filtered) {
	this->texture = &texture;
	this->size = size;

	std::ifstream fontFile(fileName.data(), std::ios::binary);
	std::vector<char> fontBuffer((std::istreambuf_iterator<char>(fontFile)), std::istreambuf_iterator<char>());

	stbtt_fontinfo info;
	int success = stbtt_InitFont(&info, (uchar*)(fontBuffer.data()), 0);
	DEBUG_ASSERT(success, "Failed to initialize font");

	float scale = stbtt_ScaleForPixelHeight(&info, size);

	int ascent;
	stbtt_GetFontVMetrics(&info, &ascent, nullptr, nullptr);
	ascent = std::round(ascent * scale);

	int atlasWidth = 0, atlasHeight = 0;
	for (char c = FirstGlyph; c < LastGlyph; c++) {
		int advance;
		stbtt_GetCodepointHMetrics(&info, c, &advance, nullptr);

		int bottom, top;
		stbtt_GetCodepointBitmapBox(&info, c, scale, scale, nullptr, &bottom, nullptr, &top);

		if (ascent + top - bottom > atlasHeight) {
			atlasHeight = ascent + top - bottom;
		}
		atlasWidth += std::round(advance * scale);
	}
	atlasWidth = pow(2, (int)log2(atlasWidth) + 1);

	uchar* atlasData = (uchar*)std::calloc(atlasWidth * atlasHeight, sizeof(uchar));
	DEBUG_ASSERT(atlasData, "Failed to allocate space for the character atlas");

	float x = 0;
	for (char c = FirstGlyph; c < LastGlyph; c++) {
		int advance, lsb;
		stbtt_GetCodepointHMetrics(&info, c, &advance, &lsb);
		advance = std::round(advance * scale);
		lsb = std::round(lsb * scale);

		int left, bottom, right, top;
		stbtt_GetCodepointBitmapBox(&info, c, scale, scale, &left, &bottom, &right, &top);

		int y = ascent + bottom;
		int byteOffset = x + (y * atlasWidth);
		stbtt_MakeCodepointBitmap(
			&info,
			&atlasData[byteOffset],
			right - left,
			top - bottom,
			atlasWidth,
			scale,
			scale,
			c);

		glyphs[c - FirstGlyph] = {right - left, advance, {x / atlasWidth, 0, (x + right - left) / atlasWidth, 1}};
		x += advance;
	}

	if (filtered) {
		texture.create(atlasWidth, atlasHeight, atlasData, TextureFormat::LinearFont());
	} else {
		texture.create(atlasWidth, atlasHeight, atlasData, TextureFormat::NearestFont());
	}
	free(atlasData);
}

void Font::destroy() {
	texture = nullptr;
}

Texture& Font::getTexture() {
	DEBUG_ASSERT(texture != nullptr, "This font hasn't been initialized");
	return *texture;
}

Glyph Font::getGlyph(int glyph) const {
	if (glyph < FirstGlyph || glyph >= LastGlyph) {
		DEBUG_WARNING("Glyph %c (%d) is out of bounds for this font", (char)glyph, glyph);
		return {0, 0, {0}};
	}
	return glyphs[glyph - FirstGlyph];
}

int Font::getSize() const {
	return size;
}

int Font::getWidth() const {
	DEBUG_ASSERT(texture != nullptr, "This font hasn't been initialized");
	return texture->getWidth();
}

int Font::getHeight() const {
	DEBUG_ASSERT(texture != nullptr, "This font hasn't been initialized");
	return texture->getHeight();
}
