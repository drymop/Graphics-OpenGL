////////////////////////////////////////////////////////////////////////////////
// Vertex shader that simply calculates the world position of vertex
////////////////////////////////////////////////////////////////////////////////

#version 330

////////////////////////////////////////////////////////////////////////////////
// Uniforms

// Properties of the scene
uniform mat4  viewProjectionMatrix;   // Transform from world to homogeneous coordinate

// Properties of the object
uniform mat4     vertexModelMatrix;   // Transform vertex from model to world coordinate
uniform mat4     normalModelMatrix;   // Transform normal from model to world coordinate

////////////////////////////////////////////////////////////////////////////////
// Vertex input/output
in  vec3 modelPos;     // Vertex position in model space
in  vec3 modelNormal;  // Vertex normal in model space
in  vec2 texCoord;  // Texture coordinate
in  vec3 modelTangent; // Tangent 

out VS_OUT {
  vec3 worldPos;     // Vertex position in world space
  vec3 worldNormal;  // Vertex normal in world space
  vec2 texCoord;     // Vertex's texture coordinate
  vec3 worldTangent; // Vertex tangent in world space
} vsOut;


void main() {
  // Calculate position and normal of vector in world coordinate
  vec4 pos = vertexModelMatrix * vec4(modelPos, 1);
  vec4 normal = normalModelMatrix * vec4(modelNormal, 0);
  vec4 tangent = vertexModelMatrix * vec4(modelTangent, 0);
  vsOut.worldPos = pos.xyz;
  vsOut.worldNormal = normalize(normal.xyz);
  vsOut.worldTangent = normalize(tangent.xyz);
  vsOut.texCoord = texCoord;

  // Calculate homogeneous coordinate of vertex for clipping
  gl_Position = viewProjectionMatrix * pos;
}
