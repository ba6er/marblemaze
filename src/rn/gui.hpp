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

		void create(la::Vec2 size, std::string_view text, la::Vec2 position, la::Vec4 color, uint align);

		std::string text;
		la::Vec2 position, size;
		la::Vec4 color;
		uint align;
	};

	class Button {

	public:
		Button();

		void create(const Label& label, la::Vec4 back, la::Vec4 selectedBack, la::Vec4 margin);

		bool isInside(float x, float y);

		bool selected;

		la::Vec2 position, size;
		la::Vec4 back, selectedBack;
	};

	class GUI {

	public:
		GUI();

		void create(rs::Shader& shader, rs::Font& font, rs::Mesh& mesh);
		void setFrame(float left, float right, float bottom, float top);

		Label& addLabel(
			const std::string& name, float size, std::string_view text, la::Vec2 position, la::Vec4 color, uint align);
		Label& addLabel(const std::string& name, float size, std::string_view text, la::Vec2 position);
		void removeLabel(std::string_view name);

		Button& addButton(
			const std::string& name,
			float size, std::string_view text, la::Vec2 position, la::Vec4 color,
			la::Vec4 back, la::Vec4 selectedBack, la::Vec4 margin);
		Button& addButton(
			const std::string& name,
			float size, std::string_view text, la::Vec2 position, la::Vec4 color,
			la::Vec4 back, la::Vec4 selectedBack, float width, la::Vec2 verticalMargin);
		Button& addButton(const std::string& name, float size, std::string_view text, la::Vec2 position);
		bool checkButtonSelected(std::string_view name, float x, float y);
		bool getButtonSelected(std::string_view name);
		void setButtonSelected(std::string_view name, bool isSelected);
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
