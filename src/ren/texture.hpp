#pragma once
#include <util.hpp>

namespace ren {

	class Texture {

	public:
		Texture();

		void create(int width, int height, void* data, bool filtered);
		void create(cstr fileName, bool filtered);
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
