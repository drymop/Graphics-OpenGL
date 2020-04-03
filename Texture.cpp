#include "Texture.h"

// image library
#include "SOIL2.h"


Texture::
Texture(const std::string& _imgFile) {
  m_textureId = SOIL_load_OGL_texture(_imgFile.c_str(),
      SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
}

void
Texture::
activate(GLenum _textureUnit) {
  if (m_textureId) {
    glActiveTexture(_textureUnit);
    glBindTexture(GL_TEXTURE_2D, m_textureId);
  }
}
