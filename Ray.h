#ifndef RAY_H_
#define RAY_H_

#include <glm/glm.hpp>

class Ray
{
  public:
    Ray(glm::vec3 _origin, glm::vec3 _dir);

    glm::vec3 getOrigin();

    glm::vec3 getDirection();

  private:
    /// Origin of the ray
    glm::vec3 m_origin;
    /// Unit vector specifying the direction of the ray
    glm::vec3 m_dir;
};

#endif // RAY_H_