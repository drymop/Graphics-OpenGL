#include "ConfigParser.h"

#include <fstream>
#include "json.hpp"

using Json = nlohmann::json;


Config
ConfigParser::
parse(const std::string& _configFileName) {
  // read file into JSON object
  std::ifstream i(_configFileName);
  Json j;
  i >> j;

  Config config;
  config.rayTracing = j.at("ray_tracing").get<bool>();
  config.screenWidth = j.at("screen_size").at(0).get<int>();
  config.screenHeight = j.at("screen_size").at(1).get<int>();
  config.sceneFile = j.at("scene").get<std::string>();
  return config;
}