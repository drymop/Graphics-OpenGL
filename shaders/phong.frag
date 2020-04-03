////////////////////////////////////////////////////////////////////////////////
// Phong shader - interpolates the normal for each fragment, then does the
// lighting computation
////////////////////////////////////////////////////////////////////////////////

#version 330

////////////////////////////////////////////////////////////////////////////////
// Uniforms

// Properties of the scene
#define MAX_LIGHTS 8
#define TYPE_POINT_LIGHT 1
#define TYPE_SPOT_LIGHT  2
#define TYPE_DIR_LIGHT   3
struct Light {
  int   type; // light type
  vec3  pos;  // position of light, in world coordinate
  vec3  dir;  // direction of light, in world coordinate
  float cutoffDot; // min dot product, aka cos(maxAngle)
  vec3  ia;   // ambient intensity
  vec3  id;   // diffuse intensity
  vec3  is;   // specular intensity
  vec3  al;   // linear attenuation (1,x,x**2 coefficients)
  float aa;   // angular attenuation
};

uniform int   numLights;          // number of lights in the scene
uniform Light lights[MAX_LIGHTS]; // lights in the scene
uniform vec3  cameraPos;          // Position of camera in the world

// Properties of the object
uniform bool hasKdMap = false;  // Is object Kd from texture, or material?
uniform bool hasKsMap = false;  // Is object Ks from texture, or material?
uniform sampler2D kdTextureSampler;
uniform sampler2D ksTextureSampler;
struct Material {
  vec3 ka;
  vec3 kd;
  vec3 ks;
  float shininess;
};
uniform Material material;        // Default material of the object, if not texture mapped


////////////////////////////////////////////////////////////////////////////////
// Vertex input/output
in  vec3 worldPos;    // Vertex position in world space
in  vec3 worldNormal; // Vertex normal in world space
in  vec2 texCoord;    // Texture coordinate
out vec4 color;       // Assigned vertex color to send to rasterizer


////////////////////////////////////////////////////////////////////////////////
/// Determine the vertex's color using Blinn-Phong illumination model
/// @param pos    Position of vertex in world space
/// @param normal Normal of vertex in world space
vec4 shadeBlinnPhong(in vec3 pos, in vec3 normal) {
  vec3 color = vec3(0.0, 0.0, 0.0);          // accumulate color
  vec3 viewDir = normalize(cameraPos - pos); // direction toward camera

  // material of the current fragment, comes from either texture of default material
  vec3 ka = material.ka;
  vec3 kd = hasKdMap? texture(kdTextureSampler, texCoord).xyz : material.kd;
  vec3 ks = hasKdMap? texture(ksTextureSampler, texCoord).xyz : material.ks;

  // add illumination from each light source
  for (int i = 0; i < numLights; i++) {
    // ambient
    color += ka * lights[i].ia;

    // calculate direction and attenuation to light of specific light types
    float attenuation = 0;
    vec3 lightDir;  // direction toward light
    if (lights[i].type == TYPE_POINT_LIGHT) {
      lightDir = lights[i].pos - pos;
      float d = length(lightDir);
      lightDir /= d;
      vec3 al = lights[i].al;
      attenuation = 1/(al[0] + al[1]*d + al[2]*d*d);
    } else if (lights[i].type == TYPE_DIR_LIGHT) {
      attenuation = 1;
      lightDir = -lights[i].dir;
    } else if (lights[i].type == TYPE_SPOT_LIGHT) {
      lightDir = lights[i].pos - pos;
      float d = length(lightDir);
      lightDir /= d;
      float dotProd = -dot(lightDir, lights[i].dir);
      if (dotProd > lights[i].cutoffDot) {
        vec3 al = lights[i].al;
        attenuation = pow(dotProd, lights[i].aa) / (al[0] + al[1]*d + al[2]*d*d);
      }
    }

    // diffuse
    color += attenuation * kd * lights[i].id * max(0.0, dot(normal, lightDir));
    // specular
    vec3 halfVec = normalize(viewDir + lightDir);
    color += attenuation 
             * ks 
             * lights[i].is 
             * pow(max(0.0, dot(normal, halfVec)), material.shininess);
  }
  color = min(color, vec3(1.0, 1.0, 1.0));
  return vec4(color, 1.0);
}


void main() {
  color = shadeBlinnPhong(worldPos, normalize(worldNormal));
}