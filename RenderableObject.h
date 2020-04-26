#ifndef RENDERABLE_OBJECT_H_
#define RENDERABLE_OBJECT_H_

#include "Material.h"
#include "Texture.h"

class RenderableObject
{
  public:
    ////////////////////////////////////////////////////////////////////////////
    /// @brief Create an object with material that can be texture mapped
    RenderableObject(const MaterialConfig& _materialConfig);

    ////////////////////////////////////////////////////////////////////////////
    /// @brief Create an object with a simple material
    RenderableObject(const Material& _material) : m_defaultMaterial(_material) {};
    
    ////////////////////////////////////////////////////////////////////////////
    /// @brief Update object between frames
    virtual void update(float deltaTime) {};

    ////////////////////////////////////////////////////////////////////////////
    /// @brief  Virtual destructor to make class abstract
    virtual ~RenderableObject() {};

    // there will be more stuff added, but for now this is just the base for
    // all objects

  protected:
    Texture  m_kdTexture;
    Texture  m_ksTexture;
    Texture  m_keTexture;
    Texture  m_normalTexture;
    Texture  m_parallaxTexture;
    Material m_defaultMaterial;
};

#endif // RENDERABLE_OBJECT_H_