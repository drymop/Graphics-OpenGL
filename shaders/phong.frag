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
uniform bool hasTransparency = false; // Any part of the object is transparent?
uniform bool hasKdMap = false;  // Is object Kd from texture, or material?
uniform bool hasKsMap = false;  // Is object Ks from texture, or material?
uniform bool hasKeMap = false;  // Is object Ke from texture, or material?
uniform bool hasNormalMap = false;    // Is object normal mapped?
uniform bool hasParallaxMap = false;  // Is object parallax mapped?
uniform sampler2D kdTextureSampler;     // diffuse mapping
uniform sampler2D ksTextureSampler;     // specular mapping
uniform sampler2D keTextureSampler;     // emission mapping
uniform sampler2D normalTextureSampler; // normal mapping
uniform sampler2D parallaxTextureSampler;  // parallax mapping

uniform float parallaxScale = 0.1;

struct Material {
  vec3 ka;
  vec3 kd;
  vec3 ks;
  vec3 ke;
  float shininess;
  float transparency;
};
uniform Material material;        // Default material of the object, if not texture mapped


////////////////////////////////////////////////////////////////////////////////
// Vertex input/output
in VS_OUT {
  vec3 worldPos;     // Vertex position in world space
  vec3 worldNormal;  // Vertex normal in world space
  vec2 texCoord;     // Vertex's texture coordinate
  vec3 worldTangent; // Vertex tangent in world space
} fsIn;

out vec4 color;       // Assigned vertex color to send to rasterizer


////////////////////////////////////////////////////////////////////////////////
/// Determine the vertex's color using Blinn-Phong illumination model
/// @param pos    Position of vertex in world space
/// @param normal Normal of vertex in world space
vec4 shadeBlinnPhong(in vec3 pos, in vec3 normal, in vec3 viewDir, in vec2 texCoord) {
  // accumulated color
  vec3 color = hasKeMap 
      ? texture(keTextureSampler, texCoord).xyz 
      : material.ke;

  // material of the current fragment, comes from either texture of default material
  vec3 kd = material.kd;
  float transparency = hasTransparency? material.transparency : 1;
  if (hasKdMap) {
    vec4 texel = texture(kdTextureSampler, texCoord);
    kd = texel.rgb;
    transparency = hasTransparency? texel.a : 1; 
  }
  vec3 ka = material.ka;
  vec3 ks = hasKsMap? texture(ksTextureSampler, texCoord).xyz : material.ks;

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
  // color = material.ke;
  return vec4(color, transparency);
}


////////////////////////////////////////////////////////////////////////////////
/// Determine the fragment's normal from normal mapping texture
vec3 calculateNormal(in vec2 texCoord, in mat3 tbnMatrix) {
  if (!hasNormalMap) {
    return normalize(fsIn.worldNormal);
  }
  // normal in tangent space
  vec3 tsNormal = texture(normalTextureSampler, fsIn.texCoord).xyz;
  tsNormal = 2 * tsNormal - vec3(1, 1, 1);
  return normalize(tbnMatrix* tsNormal);
}

////////////////////////////////////////////////////////////////////////////////
/// Determine the fragment's parallax texture coordinate
/// @param  texCoord Texture coordinate of fragment
/// @param  viewDir  View direction, in tangent space
/// @return The parallax texture coordinate
vec2 calculateParallaxTexCoord(in vec2 texCoord, in vec3 viewDir) {
  float depth = texture(parallaxTextureSampler, texCoord).x;
  return texCoord - viewDir.xy * depth * parallaxScale;
}


void main() {
  // convert to/from tangent space using TBN (tangent, bitangent, normal) matrix
  vec3 n = normalize(fsIn.worldNormal);
  vec3 t = normalize(fsIn.worldTangent);
  vec3 b = cross(n, t);
  mat3 tbnMatrix = mat3(t, b, n);

  // parallax mapping: shift the texture coordinate based on parallax map
  vec3 viewDir = normalize(cameraPos - fsIn.worldPos); // direction toward camera
  vec3 tangentViewDir = transpose(tbnMatrix) * viewDir;
  vec2 parallaxTexCoord = calculateParallaxTexCoord(fsIn.texCoord, tangentViewDir);
  
  // if texture coordinate is out of range, don't show fragment
  if (parallaxTexCoord.x < 0.0 || parallaxTexCoord.x > 1.0 
      || parallaxTexCoord.y < 0.0 || parallaxTexCoord.y > 1.0) {
    discard;
  }

  vec3 normal = calculateNormal(parallaxTexCoord, tbnMatrix);
  color = shadeBlinnPhong(fsIn.worldPos, normal, viewDir, parallaxTexCoord);
}