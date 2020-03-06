#ifndef RASTERIZER_H_
#define RASTERIZER_H_

#include "Renderer.h"

class Rasterizer : public Renderer
{
  public:
    Rasterizer(int frameWidth, int frameHeight) 
      : Renderer(frameWidth, frameHeight) 
    {}

    void render(const Scene& scene) override;
};

#endif // RASTERIZER_H_