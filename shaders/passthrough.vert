////////////////////////////////////////////////////////////////////////////////
// Simple pass through vertex shader, i.e., do not alter anything about the
// position, simply set the required gl data. Additionally, pass along the color
// data about the vertex.
////////////////////////////////////////////////////////////////////////////////

#version 330

struct Material
{
  vec3 ka;
  vec3 kd;
  vec3 ks;
  float shininess;
};

uniform  Material            material; // Material of the vertex
uniform  mat4     modelToCameraMatrix; // Transform from model to camera coordinate

     in  vec4 vpos; // Input vertex position from data
     in  vec4 vnormal; // Input vertex normal from data
flat out vec4 color; // Assigned vertex color to send to rasterizer

void main() {
  // manually scale the object down, so it fits within the clipping space
  gl_Position = modelToCameraMatrix * vpos;
  // // use normal as color
  // color = abs(vnormal);
  color = vec4(material.kd, 1);
}
