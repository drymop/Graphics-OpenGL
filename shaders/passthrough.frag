////////////////////////////////////////////////////////////////////////////////
// Simple pass through vertex shader, i.e., do not alter anything about the
// position, simply set the required gl data.
////////////////////////////////////////////////////////////////////////////////

#version 330

flat in  vec4  color; // Interpolated color from rasterization step
out vec4 fcolor; // Output fragment color

void main() {
  fcolor = color;
  // fcolor = vec4(1, 0, 0, 1);
}