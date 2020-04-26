#include "Rectangle.h"

Rectangle::
Rectangle(
  glm::vec3 _bottomLeft,
  glm::vec3 _right,
  glm::vec3 _up,
  const MaterialConfig& _material
)
  : RasterizableObject(
      generateMesh(_bottomLeft, _right, _up), 
      _material, 
      glm::mat4(1.f)
    )
{}

Mesh
Rectangle::
generateMesh(
  glm::vec3 bottomLeft,
  glm::vec3 right,
  glm::vec3 up
) {
  glm::vec3 p0 = bottomLeft;
  glm::vec3 p1 = bottomLeft + right;
  glm::vec3 p2 = bottomLeft + up + right;
  glm::vec3 p3 = bottomLeft + up;
  glm::vec3 n  = glm::normalize(glm::cross(right, up));
  glm::vec3 tangent = glm::normalize(right);
  Vertex v0 {p0, n, {0, 0}, tangent};
  Vertex v1 {p1, n, {1, 0}, tangent};
  Vertex v2 {p2, n, {1, 1}, tangent};
  Vertex v3 {p3, n, {0, 1}, tangent};
  return Mesh({v0, v1, v2, v0, v2, v3});
}