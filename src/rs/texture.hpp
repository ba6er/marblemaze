#pragma once
#include <util.hpp>
#include <string_view>

namespace rs {

	class TextureFormat {

	public:

		static TextureFormat NearestRGBA(int mipmaps = 0);
		static TextureFormat LinearRGBA(int mipmaps = 0);
		static TextureFormat NearestFont();
		static TextureFormat LinearFont();

		bool filtered;
		int textureFormat;
		int sourceFormat;
		int mipmaps;
	};

	class Texture {

	public:
		Texture();

		void create(int width, int height, void* data, TextureFormat format);
		void create(std::string_view fileName, bool filtered, int mipmaps = 4);
		void create();
		void destroy();

		void use(int index) const;

		int  getWidth() const;
		int  getHeight() const;
		uint getId() const;

	private:
		int  width, height;
		uint id;
	};
}
