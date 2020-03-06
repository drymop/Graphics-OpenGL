#ifndef CONFIG_PARSER_H_
#define CONFIG_PARSER_H_

#include <string>

/// Data read from config file
struct Config {
  bool rayTracing;
  int screenWidth;
  int screenHeight;
  std::string sceneFile;
};

class ConfigParser
{
  public:
    Config parse(const std::string& _configFileName);
};

#endif // CONFIG_PARSER_H_