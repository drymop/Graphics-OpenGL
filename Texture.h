#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <string>

// Open GL
#include "GLInclude.h"

class Texture
{
  public:
    Texture() : m_textureId(0) {};

    Texture(const std::string& _imgFile);

    bool isValid() {return m_textureId != 0;}

    void activate(GLenum _textureUnit);

  private:
    GLuint m_textureId;
};

#endif // TEXTURE_H_