#include <ren/gui.hpp>

using namespace ren;

Label::Label() : size(0), position({0, 0, 0}), text("") {}

void Label::create(float size, lin::Vec3 position, std::string_view text, TextAlignMask align) {
	this->size = size;
	this->position = position;
	this->text = std::string(text);
	this->align = align;
}

GUI::GUI() : shader(nullptr), font(nullptr), mesh(nullptr), labels() {}

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
	shader->setUniform("projection", lin::Mat4::Project2d(left, right, bottom, top, -1000, 1000));
}

Label& GUI::addLabel(const std::string& name) {
	labels[name] = Label();
	return labels.at(name);
}

Label& GUI::getLabel(std::string_view name) {
	auto value = labels.find(name);
	DEBUG_ASSERT(value != labels.end(), "No label by the name of \"%s\"", name.data());
	return value->second;
}

void GUI::display() {
	updateMesh();
}

void ren::GUI::updateMesh() {
}
