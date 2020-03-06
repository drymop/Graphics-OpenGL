#ifndef RENDERABLE_OBJECT_H_
#define RENDERABLE_OBJECT_H_

// GLM
#include "Material.h"

class RenderableObject
{
  public:
    ////////////////////////////////////////////////////////////////////////////
    /// @brief Create an object with a material
    RenderableObject(const Material& _material) 
      : m_material(_material) {};
    
    ////////////////////////////////////////////////////////////////////////////
    /// @brief  Virtual destructor to make class abstract
    virtual ~RenderableObject() {};

    Material getMaterial() const {
      return m_material;
    }

  protected:
    Material m_material;
};

#endif // RENDERABLE_OBJECT_H_