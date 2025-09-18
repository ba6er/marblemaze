#include <rn/gui.hpp>
#include <glad/glad.h>

using namespace rn;
using namespace rs;

Label::Label() : size(0), text(""), position({0}), color({0}), align(0) {}

void Label::create(float size, std::string_view text, la::Vec3 position, la::Vec3 color, uint align) {
	this->size = size;
	this->text = std::string(text);
	this->position = position;
	this->color = color;
	this->align = align;
}

void Label::create(float size, std::string_view text, la::Vec3 position) {
	create(size, text, position, {1, 1, 1});
}

GUI::GUI() : shader(nullptr), font(nullptr), mesh(nullptr), labels(), needsUpdate(false) {}

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
	shader->setUniform("projection", la::Mat4::Project2d(left, right, bottom, top, -1000, 1000));
}

Label& GUI::addLabel(const std::string& name) {
	needsUpdate = true;
	labels[name] = Label();
	return labels.at(name);
}

Label& GUI::getLabel(std::string_view name) {
	auto value = labels.find(name);
	DEBUG_ASSERT(value != labels.end(), "No label by the name of \"%s\"", name.data());
	needsUpdate = true;
	return value->second;
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

void GUI::updateMesh() {
	mesh->clear();

	for (auto& labelKV : labels) {
		Label& l = labelKV.second;
		if (l.text.empty()) {
			continue;
		}

		float hScale = l.size / font->getHeight();
		la::Vec2 offs = {0, -l.size / 4};
		if (!(l.align & TextAlign::Left)) {
			float width = 0;
			for (char c : l.text) {
				width += font->getGlyph(c).advance * hScale;
			}
			if (l.align & TextAlign::Right) {
				offs.x = -width;
			} else {
				offs.x = -width / 2;
			}
		}
		if (l.align & TextAlign::Top) {
			offs.y = 0;
		}
		if (l.align & TextAlign::Bottom) {
			offs.y = -l.size / 2;
		}

		float adv = 0;
		for (char c : l.text) {
			Glyph g = font->getGlyph(c);

			float cw = g.width * hScale;
			la::Vec2 vp = {adv + l.position.x + offs.x, l.position.y + offs.y};
			Vertex square[] = {
				{{vp.x,      vp.y,          0}, l.color, {g.uv.x, g.uv.y}, {0}},
				{{vp.x + cw, vp.y,          0}, l.color, {g.uv.z, g.uv.y}, {0}},
				{{vp.x + cw, vp.y + l.size, 0}, l.color, {g.uv.z, g.uv.w}, {0}},
				{{vp.x,      vp.y + l.size, 0}, l.color, {g.uv.x, g.uv.w}, {0}},
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
