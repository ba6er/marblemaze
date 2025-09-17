#pragma once
#include <la/linalg.hpp>
#include <rs/font.hpp>
#include <rs/mesh.hpp>
#include <rs/shader.hpp>
#include <map>
#include <string>
#include <string_view>

namespace rn {

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

		void create(float size, std::string_view text, la::Vec3 position, la::Vec3 color, uint align = Center);
		void create(float size, std::string_view text, la::Vec3 position);

		float size;
		std::string text;
		la::Vec3 position;
		la::Vec3 color;
		uint align;
	};

	class GUI {

	public:
		GUI();

		void create(rs::Shader& shader, rs::Font& font, rs::Mesh& mesh);
		void setFrame(float left, float right, float bottom, float top);

		Label& addLabel(const std::string& name);
		Label& getLabel(std::string_view name);
		void removeLabel(std::string_view name);

		void display();

	private:
		void updateMesh();

		rs::Shader* shader;
		rs::Font* font;
		rs::Mesh* mesh;

		std::map<std::string, Label, std::less<>> labels;

		bool needsUpdate;
	};
}
