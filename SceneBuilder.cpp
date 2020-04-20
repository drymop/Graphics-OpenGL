#include "SceneBuilder.h"

#include <fstream>

// test
#include <iostream>
#include <vector>

#include "Material.h"
#include "ObjFileParser.h"

// lights
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

// scene objects
#include "Circle.h"
#include "Plane.h"
#include "Portal.h"
#include "RasterizableObject.h"
#include "Rectangle.h"
#include "Sphere.h"

// particle system
#include "LineAttractor.h"
#include "ParticleDrag.h"
#include "ParticleSystem.h"
#include "ParticleGenerator.h"
#include "ParticleGravity.h"
#include "PointAttractor.h"
#include "PointGenerator.h"

using Json = nlohmann::json;
using glm::vec3, glm::mat4;
using std::make_unique, std::move, std::string, std::unique_ptr, std::vector;

vec3
getVec3(const Json& j) {
  return vec3(j.at(0).get<float>(), j.at(1).get<float>(), j.at(2).get<float>());
}

void
from_json(const Json& j, MaterialConfig& mc) {
  if (j.is_string()) {
    // name of material file
    mc = parseMaterialFile(j.get<string>());
  } else {
    Material m{}; 
    m.ka = getVec3(j.at("k_a"));
    m.kd = getVec3(j.at("k_d"));
    m.ks = getVec3(j.at("k_s"));
    if (j.find("k_r") != j.end()) {
      m.kr = getVec3(j.at("k_r"));
    }
    if (j.find("k_e") != j.end()) {
      m.ke = getVec3(j.at("k_e"));
    }
    j.at("shininess").get_to(m.shininess);
    mc.defaultMaterial = m;
    mc.hasKdMap = mc.hasKsMap = mc.hasKeMap = false;
  }
}

Scene
SceneBuilder::
buildSceneFromJsonFile(const string& _jsonFileName) {
  std::ifstream i(_jsonFileName);
  Json j;
  i >> j;
  return buildSceneFromJson(j);
}


Scene
SceneBuilder::
buildSceneFromJson(const Json& _sceneJson) {
  Scene scene;
  // lights
  Json lightsJson = _sceneJson.at("lights");
  for (auto& j : lightsJson) {
    string type = j.at("type");
    glm::vec3 ia{0, 0, 0};
    if (j.find("i_a") != j.end()) {
      ia = getVec3(j.at("i_a"));
    }
    if (type == "point") {
      scene.addLightSource(move(make_unique<PointLight>(
        getVec3(j.at("pos")),
        ia, 
        getVec3(j.at("i_d")), 
        getVec3(j.at("i_s")),
        getVec3(j.at("a_l"))
      )));
    } else if (type == "directional") {
      scene.addLightSource(move(make_unique<DirectionalLight>(
        getVec3(j.at("dir")),
        ia, 
        getVec3(j.at("i_d")), 
        getVec3(j.at("i_s"))
      )));
    } else if (type == "spot") {
      scene.addLightSource(move(make_unique<SpotLight>(
        getVec3(j.at("pos")),
        getVec3(j.at("dir")),
        j.at("angle").get<float>(),
        ia, 
        getVec3(j.at("i_d")), 
        getVec3(j.at("i_s")),
        getVec3(j.at("a_l")),
        j.at("a_a").get<float>()
      )));
    }
  }

  // objects
  Json objectsJson = _sceneJson.at("objects");
  for (auto& j : objectsJson) {
    string type = j.at("type");
    if (type == "mesh") {
      Mesh mesh = parseObjFile(j.at("obj").get<string>());
      mat4 transform = mat4(1.f);
      if (j.find("translate") != j.end()) {
        vec3 translate = getVec3(j.at("translate"));
        transform = glm::translate(transform, translate);
      }
      if (j.find("rotate") != j.end()) {
        vec3 axis = getVec3(j.at("rotate").at("axis"));
        float angle = j.at("rotate").at("angle").get<float>();
        transform = glm::rotate(transform, angle, axis);
      }
      if (j.find("scale") != j.end()) {
        vec3 scale = getVec3(j.at("scale"));
        transform = glm::scale(transform, scale);
      }
      scene.addObject(move(make_unique<RasterizableObject>(
        mesh,
        j.at("material").get<MaterialConfig>(),
        transform
      )));
    } else if (type == "sphere") {
      scene.addObject(move(make_unique<Sphere>(
        getVec3(j.at("center")), 
        j.at("radius").get<float>(), 
        j.at("material").get<MaterialConfig>(),
        m_isRayTrace
      )));
    } else if (type == "plane") {
      scene.addObject(move(make_unique<Plane>(
        getVec3(j.at("point")), 
        getVec3(j.at("normal")), 
        j.at("material").get<MaterialConfig>()
      )));
    } else if (type == "rectangle") {
      scene.addObject(move(make_unique<Rectangle>(
        getVec3(j.at("bot_left")),
        getVec3(j.at("right")),
        getVec3(j.at("up")),
        j.at("material").get<MaterialConfig>()
      )));
    } else if (type == "circle") {
      scene.addObject(move(make_unique<Circle>(
        getVec3(j.at("center")),
        j.at("radius").get<float>(), 
        getVec3(j.at("normal")), 
        j.at("material").get<MaterialConfig>()
      )));
    } else if (type == "portal") {
      scene.addObject(move(make_unique<Portal>(
        getVec3(j.at("center1")),
        j.at("radius1").get<float>(), 
        getVec3(j.at("normal1")),
        getVec3(j.at("up1")),
        getVec3(j.at("center2")),
        j.at("radius2").get<float>(), 
        getVec3(j.at("normal2")),
        getVec3(j.at("up2"))
      )));
    } else if (type == "particles") {
      buildParticleSystem(scene, j);
    }
  }


  return scene;
}

void
SceneBuilder::
buildParticleSystem(Scene& scene, const Json& json) {
  // read all generators
  vector<unique_ptr<ParticleGenerator>> gens{};
  for (auto& j : json.at("generators")) {
    string type = j.at("type");
    if (type == "point") {
      gens.push_back(move(make_unique<PointGenerator>(
        getVec3(j.at("point")),
        j.at("speed").get<float>(),
        j.at("mass").get<float>(),
        j.at("age").get<float>(),
        j.at("gen_size").get<int>(),
        j.at("period").get<float>()
      )));
    }
  }

  // read all forces
  vector<unique_ptr<ParticleForce>> forces{};
  if (json.find("forces") != json.end()) {
    const Json& fJson = json.at("forces");
    for (auto& j : fJson) {
      string type = j.at("type");
      if (type == "drag") {
        forces.push_back(move(make_unique<ParticleDrag>(j.at("k").get<float>())));
      } else if (type == "point") {
        forces.push_back(move(make_unique<PointAttractor>(
          getVec3(j.at("point")),
          j.at("g").get<float>()
        )));
      } else if (type == "line") {
        forces.push_back(move(make_unique<LineAttractor>(
          getVec3(j.at("point")),
          getVec3(j.at("dir")),
          j.at("g").get<float>()
        )));
      } else if (type == "gravity") {
        forces.push_back(move(make_unique<ParticleGravity>(
          getVec3(j.at("g"))
        )));
      }
    }
  }
  scene.addObject(move(make_unique<ParticleSystem>(
    getVec3(json.at("color")),
    json.at("g").get<float>(),
    move(gens),
    move(forces)
  )));
}
