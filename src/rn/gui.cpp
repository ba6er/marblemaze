#include <rn/gui.hpp>
#include <glad/glad.h>
#include <cmath>

using namespace rn;
using namespace rs;

Label::Label() : text(""), position({0, 0}), size({0, 0}), color({0, 0, 0, 0}), align(0) {}

void Label::create(la::Vec2 size, std::string_view text, la::Vec2 position, la::Vec4 color, uint align) {
	this->text = std::string(text);
	this->position = position;
	this->size = size;
	this->color = color;
	this->align = align;
}

Button::Button() : selected(false), position({0, 0}), size({0, 0}), back({0, 0, 0, 0}), selectedBack({0, 0, 0, 0}) {}

void Button::create(const Label& label, la::Vec4 back, la::Vec4 selectedBack, la::Vec4 margin) {
	la::Vec2 marginSize = {margin.x + margin.z, margin.y + margin.w};
	la::Vec2 marginOffset = {margin.z - margin.x, margin.w - margin.y};
	this->position = label.position + marginOffset / 2;
	this->size = label.size + marginSize;
	this->back = back;
	this->selectedBack = selectedBack;
}

bool Button::isInside(float x, float y) {
	return std::abs(position.x - x) <= (size.x / 2) && std::abs(position.y - y) <= (size.y / 2);
}

GUI::GUI() : frame({0, 0, 0, 0}), shader(nullptr), font(nullptr), mesh(nullptr), labels(), needsUpdate(false) {}

void GUI::create(Shader& shader, Font& font, Mesh& mesh) {
	this->shader = &shader;
	this->font = &font;
	this->mesh = &mesh;
}

void GUI::setFrame(float left, float right, float bottom, float top) {
	if (shader == nullptr) {
		DEBUG_WARNING("GUI doesn't have a valid shader attached");
		return;
	}
	frame = {left, bottom, right, top};
	shader->setUniform("projection", la::Mat4::Project2d(left, right, bottom, top, -1000, 1000));
}

Label& GUI::addLabel(
		const std::string& name, float size, std::string_view text, la::Vec2 position, la::Vec4 color, uint align) {

	needsUpdate = true;
	labels[name] = Label();

	float width = 0;
	for (char c : text) {
		width += font->getGlyph(c).advance * size / font->getHeight();
	}
	labels.at(name).create({width, size}, text, position, color, align);
	return labels.at(name);
}

Label& GUI::addLabel(const std::string& name, float size, std::string_view text, la::Vec2 position) {
	return addLabel(name, size, text, position, {1, 1, 1, 1}, Center);
}

void GUI::removeLabel(std::string_view name) {
	auto value = labels.find(name);
	if (value == labels.end()) {
		DEBUG_WARNING("No label by the name of \"%s\"", name.data());
		return;
	}
	labels.erase(value);
	needsUpdate = true;
}

Button& GUI::addButton(
		const std::string& name,
		float size, std::string_view text, la::Vec2 position, la::Vec4 color,
		la::Vec4 back, la::Vec4 selectedBack, la::Vec4 margin) {

	addLabel(name, size, text, position, color, Center);
	buttons[name] = Button();
	buttons.at(name).create(labels.at(name), back, selectedBack, margin);
	return buttons.at(name);
}

Button& GUI::addButton(
		const std::string& name,
		float size, std::string_view text, la::Vec2 position, la::Vec4 color,
		la::Vec4 back, la::Vec4 selectedBack, float width, la::Vec2 verticalMargin) {

	Label& l = addLabel(name, size, text, position, color, Center);
	la::Vec4 margin = {(width - l.size.x) / 2, verticalMargin.x, (width - l.size.x) / 2, verticalMargin.y};

	buttons[name] = Button();
	buttons.at(name).create(l, back, selectedBack, margin);
	return buttons.at(name);
}

Button& GUI::addButton(const std::string& name, float size, std::string_view text, la::Vec2 position) {
	return addButton(name, size, text, position, {1, 1, 1, 1}, {0, 0, 0, 1}, {1, 0, 0, 1}, {0, 0, 0, 0});
}

bool GUI::checkButton(std::string_view name, float x, float y) {
	auto value = buttons.find(name);
	DEBUG_ASSERT(value != buttons.end(), "No button by the name of \"%s\"", name.data());

	bool posInsideButton = value->second.isInside(x, y);
	if (posInsideButton != value->second.selected) {
		needsUpdate = true;
		value->second.selected = posInsideButton;
	}
	return value->second.selected;
}

void GUI::removeButton(std::string_view name) {
	auto value = buttons.find(name);
	if (value == buttons.end()) {
		DEBUG_WARNING("No button by the name of \"%s\"", name.data());
		return;
	}
	buttons.erase(value);
	needsUpdate = true;
}

void GUI::display() {
	if (needsUpdate) {
		updateMesh();
		needsUpdate = false;
	}

	glDisable(GL_DEPTH_TEST);

	font->getTexture().use(0);
	shader->use();
	mesh->draw();
}

void GUI::clear() {
	labels.clear();
	buttons.clear();
	needsUpdate = true;
}

void GUI::updateMesh() {
	mesh->clear();

	for (auto& buttonKV : buttons) {
		Button& b = buttonKV.second;

		la::Vec2 vp1 = b.position - b.size / 2;
		la::Vec2 vp2 = b.position + b.size / 2;

		la::Vec3 col = {b.back.x, b.back.y, b.back.z};
		if (b.selected) {
			col = {b.selectedBack.x, b.selectedBack.y, b.selectedBack.z};
		}
		float alpha = b.selected ? b.selectedBack.w : b.back.w;

		Vertex square[] = {
			{{vp1.x, vp1.y, 0}, col, {0, 0}, {1, alpha, 0}},
			{{vp2.x, vp1.y, 0}, col, {0, 0}, {1, alpha, 0}},
			{{vp2.x, vp2.y, 0}, col, {0, 0}, {1, alpha, 0}},
			{{vp1.x, vp2.y, 0}, col, {0, 0}, {1, alpha, 0}},
		};
		mesh->addVertex(square[0]);
		mesh->addVertex(square[1]);
		mesh->addVertex(square[2]);
		mesh->addVertex(square[2]);
		mesh->addVertex(square[3]);
		mesh->addVertex(square[0]);
	}

	for (auto& labelKV : labels) {
		Label& l = labelKV.second;
		if (l.text.empty()) {
			continue;
		}

		float scale = l.size.y / font->getHeight();
		la::Vec2 offs = {-l.size.x / 2, -l.size.y / 2};
		if (l.align & TextAlign::Left) {
			offs.x = 0;
		}
		if (l.align & TextAlign::Right){
			offs.x = -l.size.x;
		}
		if (l.align & TextAlign::Top) {
			offs.y = 0;
		}
		if (l.align & TextAlign::Bottom) {
			offs.y = -l.size.y;
		}

		float adv = 0;
		for (char c : l.text) {
			Glyph g = font->getGlyph(c);

			float cw = g.width * scale;
			float ch = l.size.y;
			la::Vec2 vp = {adv + l.position.x + offs.x, l.position.y + offs.y};
			la::Vec3 col = {l.color.x, l.color.y, l.color.z};
			Vertex square[] = {
				{{vp.x,      vp.y,      0}, col, {g.uv.x, g.uv.y}, {0, l.color.w, 0}},
				{{vp.x + cw, vp.y,      0}, col, {g.uv.z, g.uv.y}, {0, l.color.w, 0}},
				{{vp.x + cw, vp.y + ch, 0}, col, {g.uv.z, g.uv.w}, {0, l.color.w, 0}},
				{{vp.x,      vp.y + ch, 0}, col, {g.uv.x, g.uv.w}, {0, l.color.w, 0}},
			};
			mesh->addVertex(square[0]);
			mesh->addVertex(square[1]);
			mesh->addVertex(square[2]);
			mesh->addVertex(square[2]);
			mesh->addVertex(square[3]);
			mesh->addVertex(square[0]);

			adv += g.advance * scale;
		}
	}
}
