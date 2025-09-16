#pragma once
#include <lin/linalg.hpp>
#include <ren/font.hpp>
#include <ren/mesh.hpp>
#include <ren/shader.hpp>
#include <map>
#include <string>
#include <string_view>

namespace ren {

	enum TextAlign : uint {
		Center = 0,
		Left   = 1 << 0,
		Right  = 1 << 1,
		Bottom = 1 << 2,
		Top    = 1 << 3,
	};

	class Label {

	public:
		Label();

		void create(float size, std::string_view text, lin::Vec3 position, lin::Vec3 color, uint align = Center);
		void create(float size, std::string_view text, lin::Vec3 position);

		float size;
		std::string text;
		lin::Vec3 position;
		lin::Vec3 color;
		uint align;
	};

	class GUI {

	public:
		GUI();

		void create(Shader& shader, Font& font, Mesh& mesh);
		void setFrame(float left, float right, float bottom, float top);

		Label& addLabel(const std::string& name);
		Label& getLabel(std::string_view name);

		void display();

	private:
		void updateMesh();

		Shader* shader;
		Font* font;
		Mesh* mesh;

		std::map<std::string, Label, std::less<>> labels;
	};
}
