////////////////////////////////////////////////////////////////////////////////
// Goraud shading - interpolate color of all vertices
////////////////////////////////////////////////////////////////////////////////

#version 330

in  vec4  color; // Interpolated color from rasterization step
out vec4 fcolor; // Output fragment color

void main() {
  fcolor = color;
}
