// STL
#include <cmath>
#include <vector>

#include "Sphere.h"

Sphere::
Sphere(glm::vec3 _center, float _radius, const MaterialConfig& _matConfig, bool _isRayTraced)
  : RasterizableObject(
      _isRayTraced? Mesh() : generateMesh(10),
      _matConfig,
      glm::scale(glm::translate(glm::mat4(1.0f), _center), glm::vec3(_radius))
    ),
    m_center(_center), 
    m_radius(_radius)
    {};

RayHit 
Sphere::
intersectRay(Ray _ray) const {
  glm::vec3 p = _ray.getOrigin();
  glm::vec3 d = _ray.getDirection();
  glm::vec3 pMinusC = p - m_center;
  float bHalf = glm::dot(d, pMinusC);
  float c = glm::length2(pMinusC) - m_radius * m_radius;

  float discriminant = bHalf * bHalf - c;
  // no intersection
  if (discriminant <= 0) {
    return RayHit();
  }
  // return the smaller t
  float t = -bHalf - sqrt(discriminant);
  glm::vec3 hitPos = p + d * t;
  glm::vec3 normal = glm::normalize(hitPos - m_center);
  return {t, hitPos, normal, m_defaultMaterial};
}

Mesh
Sphere::
generateMesh(int prec) {
  std::vector<Vertex> vertices{};
  int numVertices = (prec + 1) * (prec + 1);
  for (int i = 0; i < numVertices; i++) { vertices.push_back({}); } // std::vector::push_back()
  // calculate triangle vertices
  for (int i = 0; i <= prec; i++) {
    for (int j = 0; j <= prec; j++) {
      float y = cos(glm::radians(180.0f - i * 180.0f / prec));
      float x = -cos(glm::radians(j*360.0f / prec)) * abs(cos(asin(y)));
      float z = sin(glm::radians(j*360.0f / prec)) * abs(cos(asin(y)));
      Vertex& v = vertices[i*(prec + 1) + j];
      v.p = {x, y, z};
      v.n = {x, y, z};
      v.t = {(float)j/prec, (float)i/prec};
    }
  }
  std::vector<Vertex> meshVerts{};
  // calculate triangle indices
  for (int i = 0; i<prec; i++) {
    for (int j = 0; j<prec; j++) {
      meshVerts.emplace_back(vertices[i*(prec + 1) + j]);
      meshVerts.emplace_back(vertices[i*(prec + 1) + j + 1]);
      meshVerts.emplace_back(vertices[(i + 1)*(prec + 1) + j]);
      meshVerts.emplace_back(vertices[i*(prec + 1) + j + 1]);
      meshVerts.emplace_back(vertices[(i + 1)*(prec + 1) + j + 1]);
      meshVerts.emplace_back(vertices[(i + 1)*(prec + 1) + j]);
    }
  }
  return Mesh(meshVerts);
}

