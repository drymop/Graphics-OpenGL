#include "SceneBuilder.h"

#include <fstream>

// test
#include <iostream>
#include <vector>

#include "Circle.h"
#include "Material.h"
#include "ObjFileParser.h"
#include "Plane.h"
#include "PointLight.h"
#include "Portal.h"
#include "Sphere.h"

using Json = nlohmann::json;
using glm::vec3;

vec3
getVec3(const Json& j) {
  return vec3(j.at(0).get<float>(), j.at(1).get<float>(), j.at(2).get<float>());
}

void
from_json(const Json& j, Material& m) {
  if (j.is_string()) {
    // name of material file
    m = parseMaterialFile(j.get<std::string>());
  } else {
    m.ka = getVec3(j.at("k_a"));
    m.kd = getVec3(j.at("k_d"));
    m.ks = getVec3(j.at("k_s"));
    m.kr = getVec3(j.at("k_r"));
    j.at("shininess").get_to(m.shininess);
  }
}

Scene
SceneBuilder::
buildSceneFromJsonFile(const std::string& _jsonFileName) {
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
    std::string type = j.at("type");
    if (type == "point") {
      scene.addLightSource(std::move(std::make_unique<PointLight>(
        getVec3(j.at("pos")), 
        getVec3(j.at("i_a")), 
        getVec3(j.at("i_d")), 
        getVec3(j.at("i_s"))
      )));
    }
  }

  // objects
  Json objectsJson = _sceneJson.at("objects");
  for (auto& j : objectsJson) {
    std::string type = j.at("type");
    if (type == "mesh") {

    } else if (type == "sphere") {
      scene.addObject(std::move(std::make_unique<Sphere>(
        getVec3(j.at("center")), 
        j.at("radius").get<float>(), 
        j.at("material").get<Material>()
      )));
    } else if (type == "plane") {
      scene.addObject(std::move(std::make_unique<Plane>(
        getVec3(j.at("point")), 
        getVec3(j.at("normal")), 
        j.at("material").get<Material>()
      )));
    } else if (type == "circle") {
      scene.addObject(std::move(std::make_unique<Circle>(
        getVec3(j.at("center")),
        j.at("radius").get<float>(), 
        getVec3(j.at("normal")), 
        j.at("material").get<Material>()
      )));
    } else if (type == "portal") {
      scene.addObject(std::move(std::make_unique<Portal>(
        getVec3(j.at("center1")),
        j.at("radius1").get<float>(), 
        getVec3(j.at("normal1")),
        getVec3(j.at("up1")),
        getVec3(j.at("center2")),
        j.at("radius2").get<float>(), 
        getVec3(j.at("normal2")),
        getVec3(j.at("up2"))
      )));
    }
  }

  return scene;
}