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

using Json = nlohmann::json;
using glm::vec3, glm::mat4;

vec3
getVec3(const Json& j) {
  return vec3(j.at(0).get<float>(), j.at(1).get<float>(), j.at(2).get<float>());
}

void
from_json(const Json& j, MaterialConfig& mc) {
  if (j.is_string()) {
    // name of material file
    mc = parseMaterialFile(j.get<std::string>());
  } else {
    Material m{}; 
    m.ka = getVec3(j.at("k_a"));
    m.kd = getVec3(j.at("k_d"));
    m.ks = getVec3(j.at("k_s"));
    if (j.find("k_r") != j.end()) {
      m.kr = getVec3(j.at("k_r"));
    }
    j.at("shininess").get_to(m.shininess);
    mc.defaultMaterial = m;
    mc.hasKdMap = mc.hasKsMap = false;
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
    glm::vec3 ia{0, 0, 0};
    if (j.find("i_a") != j.end()) {
      ia = getVec3(j.at("i_a"));
    }
    if (type == "point") {
      scene.addLightSource(std::move(std::make_unique<PointLight>(
        getVec3(j.at("pos")),
        ia, 
        getVec3(j.at("i_d")), 
        getVec3(j.at("i_s")),
        getVec3(j.at("a_l"))
      )));
    } else if (type == "directional") {
      scene.addLightSource(std::move(std::make_unique<DirectionalLight>(
        getVec3(j.at("dir")),
        ia, 
        getVec3(j.at("i_d")), 
        getVec3(j.at("i_s"))
      )));
    } else if (type == "spot") {
      scene.addLightSource(std::move(std::make_unique<SpotLight>(
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
    std::string type = j.at("type");
    if (type == "mesh") {
      Mesh mesh = parseObjFile(j.at("obj").get<std::string>());
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
      scene.addObject(std::move(std::make_unique<RasterizableObject>(
        mesh,
        j.at("material").get<MaterialConfig>(),
        transform
      )));
    } else if (type == "sphere") {
      scene.addObject(std::move(std::make_unique<Sphere>(
        getVec3(j.at("center")), 
        j.at("radius").get<float>(), 
        j.at("material").get<MaterialConfig>()
      )));
    } else if (type == "plane") {
      scene.addObject(std::move(std::make_unique<Plane>(
        getVec3(j.at("point")), 
        getVec3(j.at("normal")), 
        j.at("material").get<MaterialConfig>()
      )));
    } else if (type == "rectangle") {
      scene.addObject(std::move(std::make_unique<Rectangle>(
        getVec3(j.at("bot_left")),
        getVec3(j.at("right")),
        getVec3(j.at("up")),
        j.at("material").get<MaterialConfig>()
      )));
    } else if (type == "circle") {
      scene.addObject(std::move(std::make_unique<Circle>(
        getVec3(j.at("center")),
        j.at("radius").get<float>(), 
        getVec3(j.at("normal")), 
        j.at("material").get<MaterialConfig>()
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