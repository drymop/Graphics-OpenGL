#ifndef CAMERA_H_
#define CAMERA_H_ 

// Vector library
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/ext.hpp>

class Camera
{
  public:
    ////////////////////////////////////////////////////////////////////////////
    /// @brief Contruct a camera at origin, looking at the -z direction
    Camera() : Camera({0.f, 0.f, 0.f}, {0.f, 0.f, -1.f}, {0.f, 1.f, 0.f}) {};

    ////////////////////////////////////////////////////////////////////////////
    /// @brief Contruct a camera at specified position and orientation
    Camera(glm::vec3 _eye, glm::vec3 _at, glm::vec3 _up)
      : m_eye(_eye),
        m_dirty(true)
    {
      setOrientation(_at, _up);
    }

    glm::vec3 getEye()   const noexcept { return m_eye;   }
    glm::vec3 getUp()    const noexcept { return m_up;    }
    glm::vec3 getAt()    const noexcept { return m_at;    }
    glm::vec3 getRight() const noexcept { return m_right; }

    glm::mat4 getViewMatrix() const {
      if (m_dirty) {
        // only update view matrix if needed
        m_viewMatrix = glm::lookAt(m_eye, m_eye + m_at, m_up);
        m_dirty = false;
      }
      return m_viewMatrix;
    }

    void setPosition(glm::vec3 _eye) {
      m_eye = _eye;
    }

    void setOrientation(glm::vec3 _at, glm::vec3 _up) {
      m_at = glm::normalize(_at);
      m_right = glm::normalize(glm::cross(m_at, _up));
      m_up = glm::cross(m_right, m_at);
      m_dirty = true;
    }

  private:
    glm::vec3 m_eye;
    glm::vec3 m_at; 
    glm::vec3 m_up; 
    glm::vec3 m_right;
    mutable glm::mat4 m_viewMatrix;
    mutable bool m_dirty; ///< Need to update view matrix or not
};

#endif // CAMERA_H_