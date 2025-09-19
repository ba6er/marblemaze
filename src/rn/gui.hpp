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

		void create(la::Vec2 size, std::string_view text, la::Vec3 position, la::Vec3 color, uint align);
		void create(la::Vec2 size, std::string_view text, la::Vec3 position);

		std::string text;
		la::Vec2 size;
		la::Vec3 position;
		la::Vec3 color;
		uint align;
	};

	class Button {

	public:
		Button();

		void create(const Label& label, la::Vec3 back, la::Vec3 selectedBack, la::Vec2 margin);

		bool isInside(float x, float y);

		bool selected;

		la::Vec3 back, selectedBack, position;
		la::Vec2 size;
	};

	class GUI {

	public:
		GUI();

		void create(rs::Shader& shader, rs::Font& font, rs::Mesh& mesh);
		void setFrame(float left, float right, float bottom, float top);

		Label& addLabel(
			const std::string& name, float size, std::string_view text, la::Vec3 position, la::Vec3 color, uint align);
		Label& addLabel(const std::string& name, float size, std::string_view text, la::Vec3 position);
		void removeLabel(std::string_view name);

		Button& addButton(
			const std::string& name,
			float size, std::string_view text, la::Vec3 position, la::Vec3 color,
			la::Vec3 back, la::Vec3 selectedBack, la::Vec2 margin);
		Button& addButton(const std::string& name, float size, std::string_view text, la::Vec3 position);
		bool checkButton(std::string_view name, float x, float y);
		void removeButton(std::string_view name);

		void display();
		void clear();

	private:
		void updateMesh();

		la::Vec4 frame;

		rs::Shader* shader;
		rs::Font* font;
		rs::Mesh* mesh;

		std::map<std::string, Label, std::less<>> labels;
		std::map<std::string, Button, std::less<>> buttons;

		bool needsUpdate;
	};
}
