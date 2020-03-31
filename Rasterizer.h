#ifndef RASTERIZER_H_
#define RASTERIZER_H_

#include <string>

#include "Renderer.h"


class Rasterizer : public Renderer
{
  public:
    Rasterizer(int frameWidth, int frameHeight);

    void initScene(Scene& scene) override;

    void render(const Scene& scene) override;

  private:
    GLuint m_program; ///< Shader program ID

    ////////////////////////////////////////////////////////////////////////////
    /// Helper method to get uniform location from name
    GLint getUniformLocation(const std::string& uniformName);
};

#endif // RASTERIZER_H_