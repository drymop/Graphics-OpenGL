#include "Scene.h"

#include <limits>

const float Scene::SELF_INTERSECTION_BIAS = 1e-3f;

void
Scene::
addObject(std::unique_ptr<RenderableObject> _object) {
  m_objects.push_back(std::move(_object));
}

void
Scene::
addLightSource(std::unique_ptr<LightSource> _light) {
  m_lights.push_back(std::move(_light));
}

RayTracableObject*
Scene::
firstRayHit(Ray _ray, RayHit* _hitInfo) const {
  RayHit firstHit;
  firstHit.t = std::numeric_limits<float>::infinity();
  RayTracableObject* firstObj = nullptr;
  for(auto& renderableObj : m_objects) {
    RayTracableObject* obj = dynamic_cast<RayTracableObject*>(renderableObj.get());
    if (obj == nullptr) {
      // object is not ray-tracable
      continue;
    }
    RayHit hit = obj->intersectRay(_ray);
    if (hit.t > SELF_INTERSECTION_BIAS && hit.t < firstHit.t) {
      firstHit = hit;
      firstObj = obj;
    }
  }
  *_hitInfo = firstHit;
  return firstObj;
}

std::vector<LightSource*>
Scene::
lightSources() const {
  std::vector<LightSource*> lights;
  for (auto& light : m_lights) {
    lights.emplace_back(light.get());
  }
  return lights;
}

std::vector<RasterizableObject*>
Scene::
rasterizableObjects() const {
  std::vector<RasterizableObject*> objs;
  for (auto& obj : m_objects) {
    RasterizableObject* rasterizable = dynamic_cast<RasterizableObject*>(obj.get());
    if (rasterizable != nullptr) {
      objs.emplace_back(rasterizable);
    }
  }
  return objs;
}

void
Scene::
update(float deltaTime) {
  for(auto& obj : m_objects) {
    obj->update(deltaTime);
  }
}
