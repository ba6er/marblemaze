#include <rn/gui.hpp>
#include <glad/glad.h>
#include <cmath>

using namespace rn;
using namespace rs;

Label::Label() : text(""), size({0, 0}), position({0, 0, 0}), color({0, 0, 0}), align(0) {}

void Label::create(la::Vec2 size, std::string_view text, la::Vec3 position, la::Vec3 color, uint align) {
	this->size = size;
	this->text = std::string(text);
	this->position = position;
	this->color = color;
	this->align = align;
}

void Label::create(la::Vec2 size, std::string_view text, la::Vec3 position) {
	create(size, text, position, {1, 1, 1}, Center);
}

Button::Button() : selected(false), back({0, 0, 0}), selectedBack({0, 0, 0}), position({0, 0, 0}), size({0, 0}) {}

void Button::create(const Label& label, la::Vec3 back, la::Vec3 selectedBack, la::Vec2 margin) {
	this->back = back;
	this->selectedBack = selectedBack;
	this->position = label.position;
	this->size = label.size + margin * 2;
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
		const std::string& name, float size, std::string_view text, la::Vec3 position, la::Vec3 color, uint align) {
	needsUpdate = true;
	labels[name] = Label();

	float width = 0;
	for (char c : text) {
		width += font->getGlyph(c).advance * size / font->getHeight();
	}
	labels.at(name).create({width, size}, text, position, color, align);
	return labels.at(name);
}

Label& GUI::addLabel(const std::string& name, float size, std::string_view text, la::Vec3 position) {
	return addLabel(name, size,text, position, {1, 1, 1}, Center);
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
		float size, std::string_view text, la::Vec3 position, la::Vec3 color,
		la::Vec3 back, la::Vec3 selectedBack, la::Vec2 margin) {
	needsUpdate = true;
	labels[name] = Label();
	buttons[name] = Button();

	float width = 0;
	for (char c : text) {
		width += font->getGlyph(c).advance * size / font->getHeight();
	}
	labels.at(name).create({width, size}, text, position, color, Center);
	buttons.at(name).create(labels.at(name), back, selectedBack, margin);
	return buttons.at(name);
}

Button& GUI::addButton(const std::string& name, float size, std::string_view text, la::Vec3 position) {
	return addButton(name, size, text, position, {1, 1, 1}, {0, 0, 0}, {1, 0, 0}, {0, 0});
}

bool GUI::checkButton(std::string_view name, float x, float y) {
	auto value = buttons.find(name);
	DEBUG_ASSERT(value != buttons.end(), "No button by the name of \"%s\"", name.data());

	value->second.selected = value->second.isInside(x, y);
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

	for (auto& labelKV : labels) {
		Label& l = labelKV.second;
		if (l.text.empty()) {
			continue;
		}

		float hScale = l.size.y / font->getHeight();
		la::Vec2 offs = {-l.size.x / 2, -l.size.y / 4};
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
			offs.y = -l.size.y / 2;
		}

		float adv = 0;
		for (char c : l.text) {
			Glyph g = font->getGlyph(c);

			float cw = g.width * hScale;
			float ch = l.size.y;
			la::Vec2 vp = {adv + l.position.x + offs.x, l.position.y + offs.y};
			Vertex square[] = {
				{{vp.x,      vp.y,      0}, l.color, {g.uv.x, g.uv.y}, {0, 0, 0}},
				{{vp.x + cw, vp.y,      0}, l.color, {g.uv.z, g.uv.y}, {0, 0, 0}},
				{{vp.x + cw, vp.y + ch, 0}, l.color, {g.uv.z, g.uv.w}, {0, 0, 0}},
				{{vp.x,      vp.y + ch, 0}, l.color, {g.uv.x, g.uv.w}, {0, 0, 0}},
			};
			mesh->addVertex(square[0]);
			mesh->addVertex(square[1]);
			mesh->addVertex(square[2]);
			mesh->addVertex(square[2]);
			mesh->addVertex(square[3]);
			mesh->addVertex(square[0]);

			adv += g.advance * hScale;
		}
	}
}
