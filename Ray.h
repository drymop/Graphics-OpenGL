#ifndef RAY_H_
#define RAY_H_

#include <glm/glm.hpp>

class Ray
{
  public:
    Ray(glm::vec3 _origin, glm::vec3 _dir)
      : m_origin(_origin), m_dir(glm::normalize(_dir)) {};

    glm::vec3 getOrigin() const {
      return m_origin;
    }

    glm::vec3 getDirection() const {
      return m_dir;
    }

  private:
    /// Origin of the ray
    glm::vec3 m_origin;
    /// Unit vector specifying the direction of the ray
    glm::vec3 m_dir;
};

#endif // RAY_H_