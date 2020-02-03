#include "Scene.h"

#include <limits>

const float Scene::k_selfIntersectionBias = 1e-3f;

void
Scene::
addObject(std::unique_ptr<RenderableObject> _object) {
  m_objects.push_back(std::move(_object));
}

void
Scene::
setAmbientLight(glm::vec3 _intensity) {
  ambient_intensity = _intensity;
}

glm::vec3
Scene::
getAmbientLight() {
  return ambient_intensity;
}


void
Scene::
addLightSource(std::unique_ptr<LightSource> _light) {
  m_lights.push_back(std::move(_light));
}

RenderableObject*
Scene::
firstRayHit(Ray _ray, RayHit* _hitInfo) {
  RayHit firstHit;
  firstHit.t = std::numeric_limits<float>::infinity();
  int firstHitIndex = -1;
  for (int i = 0; i < m_objects.size(); i++) {
    RayHit hit = m_objects[i]->intersectRay(_ray);
    if (hit.t > k_selfIntersectionBias && hit.t < firstHit.t) {
      firstHit = hit;
      firstHitIndex = i;
    }
  }
  if (firstHitIndex < 0) {
    // no object hit by the ray
    return nullptr;
  }
  *_hitInfo = firstHit;
  return m_objects[firstHitIndex].get();
}

std::vector<LightSource*>
Scene::
lightSources() {
  std::vector<LightSource*> lights;
  for (auto& light : m_lights) {
    lights.emplace_back(light.get());
  }
  return lights;
}
