#include <gm/scene.hpp>

using namespace gm;

void Scene::updatePhysics(float deltaTime) {
	marble.position += marble.velocity * deltaTime;
}
