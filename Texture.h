#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <string>

// Open GL
#include "GLInclude.h"

class Texture
{
  public:
    Texture() : m_textureId(0), m_texData(nullptr) {};

    Texture(const std::string& _imgFile);

    ~Texture();

    // Since texture owns resource, cannot copy it
    Texture(const Texture& t) = delete;

    Texture& operator=(const Texture& t) = delete;

    // But can move texture
    Texture(Texture&& t);

    Texture& operator=(Texture&& t);

    bool isValid() const noexcept {
      return m_textureId != 0 && m_texData != nullptr;
    }

    void activate(GLenum _textureUnit) const;

    glm::vec3 sample(glm::vec2 _texCoord) const;

  private:
    /// Rasterizer texture ID
    GLuint m_textureId;
    /// Texture data for ray tracer
    unsigned char* m_texData;
    int m_width;
    int m_height;
};

#endif // TEXTURE_H_