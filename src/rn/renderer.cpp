#include <rn/renderer.hpp>
#include <glad/glad.h>

using namespace rn;

void Renderer::clear(float red, float green, float blue) {
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glClearColor(red, green, blue, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::render(Renderable& renderable) {
	renderable.material->setUniform("model", renderable.transform);
	renderable.material->setUniform("view", la::Mat4::Identity());
	renderable.material->setUniform("projection", la::Mat4::Identity());
	renderable.material->use();
	renderable.mesh->draw();
}

void Renderer::render(const Camera& camera, const std::vector<Renderable>& renderables, const Light& light) {
	la::Mat4 view = camera.getViewMatrix();
	la::Mat4 projection = camera.getProjectionMatrix();

	for (auto& r : renderables) {
		r.material->setUniform("model", r.transform);
		r.material->setUniform("view", view);
		r.material->setUniform("projection", projection);

		r.material->setUniform("uLight.position", light.position);
		r.material->setUniform("uLight.ambient", light.ambient);
		r.material->setUniform("uLight.diffuse", light.diffuse);
		r.material->setUniform("uLight.specular", light.specular);

		r.material->setUniform("uViewPosition", camera.getPosition());

		r.material->use();
		r.mesh->draw();
	}
}

void Renderer::resizeFrame(int width, int height) {
	glViewport(0, 0, width, height);
}
