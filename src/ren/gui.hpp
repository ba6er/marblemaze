#pragma once
#include <lin/linalg.hpp>
#include <ren/font.hpp>
#include <ren/mesh.hpp>
#include <ren/shader.hpp>
#include <map>
#include <string>
#include <string_view>

namespace ren {

	class Label {

	public:
		Label();

		void create(float size, lin::Vec3 position, std::string_view text);

		float size;
		lin::Vec3 position;
		std::string text;
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
