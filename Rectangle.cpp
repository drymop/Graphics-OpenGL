#include "Rectangle.h"
#include <iostream>

using glm::vec3, glm::vec4, glm::mat4, glm::normalize, glm::cross;


Rectangle::
Rectangle(
  vec3 _bottomLeft,
  vec3 _right,
  vec3 _up,
  const MaterialConfig& _material
)
  : RasterizableObject(
      generateMesh(), 
      _material, 
      generateTransform(_bottomLeft, _right, _up)
    )
{}

Mesh
Rectangle::
generateMesh() {
  vec3 n       = {0, 0, 1};
  vec3 tangent = {1, 0, 0}; 
  Vertex v0 {{0, 0, 0}, n, {0, 0}, tangent};
  Vertex v1 {{1, 0, 0}, n, {1, 0}, tangent};
  Vertex v2 {{1, 1, 0}, n, {1, 1}, tangent};
  Vertex v3 {{0, 1, 0}, n, {0, 1}, tangent};
  return Mesh({v0, v1, v2, v0, v2, v3});
}

mat4
Rectangle::
generateTransform(
  vec3 _bottomLeft,
  vec3 _right,
  vec3 _up
) {
  vec4 right = vec4(_right, 0);
  vec4 up = vec4(_up, 0);
  vec4 n = vec4(normalize(cross(_right, _up)), 0);
  vec4 translate = vec4(_bottomLeft, 1);
  return mat4(right, up, n, translate);
}