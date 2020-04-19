#include "RenderableObject.h"

#include <utility>
#include <iostream>

RenderableObject::
RenderableObject(const MaterialConfig& _config)
  : m_defaultMaterial(_config.defaultMaterial)
{
  if (_config.hasKdMap) {
    m_kdTexture = std::move(Texture(_config.kdTextureFile));
  }
  if (_config.hasKsMap) {
    m_ksTexture = std::move(Texture(_config.ksTextureFile));
  }
  if (_config.hasKeMap) {
    m_keTexture = std::move(Texture(_config.keTextureFile));
  }
}    
