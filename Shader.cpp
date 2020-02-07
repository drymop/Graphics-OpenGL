#include "Shader.h"

glm::vec3
Shader::
shade(const Scene& scene, Ray ray, int maxRecursion) {
  RayHit firstHit;
  RenderableObject* hitObj = scene.firstRayHit(ray, &firstHit);
  if (!hitObj) {
    // black if nothing is hit by the ray
    return glm::vec4();
  }
  Material material = hitObj->getMaterial();
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
Shader::
shadeSurface(const Scene& scene, glm::vec3 pos, glm::vec3 normal, glm::vec3 viewDir, const Material& material) {
  glm::vec3 color(0, 0, 0);

  // for each light source, add the ambient, diffuse and specular lighting
  for (auto& lightSource : scene.lightSources()) {
    LightRay light = lightSource->getLightRay(pos);
    // ambient light
    color += material.ka * light.intensityAmbient;
    // make sure not blocked by other objects before adding diffuse and specular
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
