#ifndef MATERIAL_H_
#define MATERIAL_H_

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
};

#endif // MATERIAL_H_