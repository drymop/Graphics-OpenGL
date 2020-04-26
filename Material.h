#ifndef MATERIAL_H_
#define MATERIAL_H_

#include <string>
#include <glm/glm.hpp>


////////////////////////////////////////////////////////////////////////////////
/// Contains paramters used for Blinn-Phong shading of a surface
struct Material {
  /// Reflection of ambient light
  glm::vec3 ka;
  /// Reflection of diffuse light
  glm::vec3 kd;
  /// Reflection of specular light
  glm::vec3 ks;
  /// Affect the size of specular spot
  float shininess;
  /// Reflection of light from other object, used for mirror like surface
  glm::vec3 kr;
  /// Emission of light
  glm::vec3 ke;
};

////////////////////////////////////////////////////////////////////////////////
/// Configure the material of a surface, either through texture mapping or
/// default material
struct MaterialConfig {
  bool hasKdMap;
  bool hasKsMap;
  bool hasKeMap;
  bool hasNormalMap;
  std::string kdTextureFile;
  std::string ksTextureFile;
  std::string keTextureFile;
  std::string normalTextureFile;
  Material defaultMaterial;
};

#endif // MATERIAL_H_