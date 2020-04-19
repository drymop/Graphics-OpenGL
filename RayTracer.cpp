#include "RayTracer.h"

RayTracer::
RayTracer(int width, int height) 
  : Renderer(width, height)
{
  m_frame = std::make_unique<glm::vec4[]>(m_width * m_height);
}

void 
RayTracer::
setFrameSize(int width, int height) {
  Renderer::setFrameSize(width, height);
  m_frame = std::make_unique<glm::vec4[]>(m_width * m_height);
}


void 
RayTracer::
render(const Scene& scene) {
  int pixel = 0;
  for (int j = 0; j < m_height; j++) {
    for (int i = 0; i < m_width; i++) {
      m_frame[pixel++] = renderPixel(scene, i, j);      
    }
  }
  glDrawPixels(m_width, m_height, GL_RGBA, GL_FLOAT, m_frame.get());
}

glm::vec4
RayTracer::
renderPixel(const Scene& scene, int i, int j) {
  glm::vec3 color(0.f, 0.f, 0.f);
  for (auto& jitter : ANTI_ALIAS_JITTERS[m_hasAntiAlias]) {
    // cast ray
    Ray ray = m_view->castRay(scene.getCamera(), i, j, jitter.x, jitter.y);
    color += shade(scene, ray, MAX_RAY_RECURSION);
  }
  color /= ANTI_ALIAS_JITTERS[m_hasAntiAlias].size();
  return glm::vec4(color, 1);
}


glm::vec3
RayTracer::
shade(const Scene& scene, Ray ray, int maxRecursion) {
  RayHit firstHit;
  RayTracableObject* hitObj = scene.firstRayHit(ray, &firstHit);
  if (!hitObj) {
    // black if nothing is hit by the ray
    return glm::vec4();
  }
  Material material = firstHit.material;
  // shade with Blinn-Phong
  glm::vec3 color = shadeSurface(scene, firstHit.position, firstHit.normal, ray.getDirection(), material);
  // add reflection for mirror-like material
  if (maxRecursion > 0 && material.kr != glm::vec3(0, 0, 0)) {
    glm::vec3 reflectDir = ray.getDirection() - 2.f * glm::dot(ray.getDirection(), firstHit.normal) * firstHit.normal;
    Ray reflectRay(firstHit.position, reflectDir);
    color += material.kr * shade(scene, reflectRay, maxRecursion - 1);
  }
  return color;
}

glm::vec3
RayTracer::
shadeSurface(const Scene& scene, glm::vec3 pos, glm::vec3 normal, glm::vec3 viewDir, const Material& material) {
  glm::vec3 color = material.ke; // object can emit light from surface

  // for each light source, add the ambient, diffuse and specular lighting
  for (auto& lightSource : scene.lightSources()) {
    LightRay light = lightSource->getLightRay(pos);
    // ambient light
    color += material.ka * light.intensityAmbient;
    // if light doesn't hit this object, continue on
    if (light.direction == glm::vec3(0, 0, 0)) {
      continue;
    }
    // if blocked by other objects, continue on
    Ray towardLight(pos, -light.direction);
    RayHit hitInfo;
    if (scene.firstRayHit(towardLight, &hitInfo)) {
      if (hitInfo.t < light.distance) {
        continue;
      }
    }
    // diffuse
    color += material.kd * light.intensityDiffuse 
        * std::max(0.f, -glm::dot(normal, light.direction));
    // specular
    glm::vec3 halfVec = -glm::normalize(viewDir + light.direction);
    color += material.ks * light.intensitySpecular
        * std::pow(std::max(0.f, glm::dot(normal, halfVec)), material.shininess);
  }
  return glm::min(color, glm::vec3(1, 1, 1)); // make sure color doesn't exceed 1
}
