#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <string>

// Open GL
#include "GLInclude.h"

class Texture
{
  public:
    Texture(const std::string& _imgFile);

    void activeTexture(GLenum _textureUnit);

  private:
    GLuint m_textureId;
};

#endif // TEXTURE_H_