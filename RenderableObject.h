#ifndef RENDERABLE_OBJECT_H_
#define RENDERABLE_OBJECT_H_

// GLM
#include <glm/glm.hpp>

#include "Material.h"


class RenderableObject
{
  using glm::mat4;

  public:
    ////////////////////////////////////////////////////////////////////////////
    /// @brief Create an object with a material and default position/orientaion
    RenderableObject(Material _material) 
      : RenderableObject(_material, mat4(1.f)) {};
    
    ////////////////////////////////////////////////////////////////////////////
    /// @brief Create an object with a material and a position/orientation
    RenderableObject(Material _material, mat4 _transform) 
      : m_material(_material), m_transform(_transform) {};
    
    ////////////////////////////////////////////////////////////////////////////
    /// @brief Pure virtual destructor to make class abstract
    virtual ~RenderableObject() = 0;

    Material getMaterial() const {
      return m_material;
    }

  protected:
    glm::mat4 m_transform;
    Material m_material;
};

RenderableObject::~RenderableObject() {};

#endif // RENDERABLE_OBJECT_H_