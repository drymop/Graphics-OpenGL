////////////////////////////////////////////////////////////////////////////////
// Simple pass through vertex shader, i.e., do not alter anything about the
// position, simply set the required gl data. Additionally, pass along the color
// data about the vertex.
////////////////////////////////////////////////////////////////////////////////

#version 330

 in vec4    vpos; // Input vertex position from data
 in vec4 vnormal; // Input vertex normal from data
flat out vec4   color; // Assigned vertex color to send to rasterizer

void main() {
  // manually scale the object down, so it fits within the clipping space
  gl_Position = vpos * vec4(0.04, 0.04, 0.04, 1.0);
  color = abs(vnormal);
}
