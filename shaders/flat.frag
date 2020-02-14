////////////////////////////////////////////////////////////////////////////////
// Flat shading - using the color of only one vertex for the entire fragment
////////////////////////////////////////////////////////////////////////////////

#version 330

flat in  vec4  color; // Color of one vertex from rasterization step
     out vec4 fcolor; // Output fragment color

void main() {
  fcolor = color;
}
