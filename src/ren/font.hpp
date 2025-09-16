#pragma once
#include <lin/linalg.hpp>
#include <ren/texture.hpp>
#include <string_view>

namespace ren {

	class Glyph {

	public:
		int width, advance;
		lin::Vec4 uv;
	};

	class Font {

	public:
		Font();

		void create(Texture* texture, std::string_view fileName, int size, bool filtered);
		void destroy();

		Texture& getTexture();
		Glyph getGlyph(int glyph) const;
		int getSize() const;
		int getWidth() const;
		int getHeight() const;

	private:
		static inline constexpr int FirstGlyph = ' '; // ' ' = 32
		static inline constexpr int LastGlyph = '~' + 1;  // '~' = 126
		static inline constexpr int NumGlyphs = LastGlyph - FirstGlyph;

		int size;
		Texture* texture;
		Glyph glyphs[NumGlyphs];

	};
}
