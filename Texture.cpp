#include "Texture.h"

#include <cmath>

// image library
#include "SOIL2.h"

#include <iostream>
using namespace std;
Texture::
Texture(const std::string& _imgFile) {
  m_textureId = SOIL_load_OGL_texture(_imgFile.c_str(),
      SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
  m_texData = SOIL_load_image(
      _imgFile.c_str(), &m_width, &m_height, 0, SOIL_LOAD_RGB);
  if (!isValid()) {
    cout << "Fucked: " << m_textureId <<" "<<_imgFile<< endl;
  }
}

Texture::
Texture(Texture&& t) 
  : m_textureId(t.m_textureId),
    m_texData(t.m_texData),
    m_width(t.m_width),
    m_height(t.m_height)
{
  t.m_textureId = 0;
  t.m_texData = nullptr;
}

Texture&
Texture::
operator=(Texture&& t) {
  m_textureId = t.m_textureId;
  m_texData = t.m_texData;
  m_width = t.m_width;
  m_height = t.m_height;
  t.m_textureId = 0;
  t.m_texData = nullptr;
}

Texture::
~Texture() {
  // free memory used by texture
  if(m_textureId) {
    glDeleteTextures(1, &m_textureId);
  }
  if (m_texData != nullptr) {
    SOIL_free_image_data(m_texData);
  }
}

void
Texture::
activate(GLenum _textureUnit) const {
  if (isValid()) {
    glActiveTexture(_textureUnit);
    glBindTexture(GL_TEXTURE_2D, m_textureId);
  }
}

glm::vec3
Texture::
sample(glm::vec2 _texCoord) const {
  if(!isValid()) {
    return {0, 0, 0};
  }
  // tile
  float s = fmod(_texCoord.x, 1.0f);
  float t = fmod(_texCoord.y, 1.0f);
  int texX = round(s * m_width);
  int texY = m_height - round(t * m_height); // invert Y coordinate
  int pix = (texY * m_width + texX) * 3;
  float r = m_texData[pix] / 255.0;
  float g = m_texData[pix+1] / 255.0;
  float b = m_texData[pix+2] / 255.0;
  return {r, g, b};
}
