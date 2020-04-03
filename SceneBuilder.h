#ifndef SCENE_BUILDER_H_
#define SCENE_BUILDER_H_

// GLM
#include <glm/glm.hpp>
// Json parsing library
#include "json.hpp"

#include "Scene.h"


class SceneBuilder
{
  public:
    SceneBuilder(bool _isRayTrace) : m_isRayTrace(_isRayTrace) {};

    Scene buildSceneFromJsonFile(const std::string& _jsonFileName);

    Scene buildSceneFromJson(const nlohmann::json& _sceneJson);

  private:
    bool m_isRayTrace;
};

#endif // SCENE_BUILDER_H_