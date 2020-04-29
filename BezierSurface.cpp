#include "BezierSurface.h"
#include <iostream>

using glm::vec2, glm::vec3;

BezierSurface::
BezierSurface(const std::vector<glm::vec3>& _controlPoints,
              const MaterialConfig& _materialConfig,
              const glm::mat4& _transform)
  : RasterizableObject(
      generateMesh(_controlPoints, 10),
      _materialConfig,
      _transform
    )
{};

Mesh
BezierSurface::
generateMesh(const std::vector<glm::vec3>& controlPoints, int prec) {
  // precompute B_i(t) for t in the interval 0 to 1, divided into prec segments
  float b[4][prec+1];
  for (int i = 0; i <= prec; i++) {
    float t = i / (float)prec;
    float t2 = t * t;
    float t3 = t2 * t;
    float s = 1-t;
    float s2 = s * s;
    float s3 = s2 * s;
    b[0][i] = s3;
    b[1][i] = 3 * t * s2;
    b[2][i] = 3 * t2 * s;
    b[3][i] = t3;
  }
  // Divide the surface into a grid of (prec+1)*(prec+1) points
  Vertex verts[prec+1][prec+1];
  for(int u = 0; u <= prec; u++) {
    for (int v = 0; v <= prec; v++) {
      // compute the point (u, v)
      Vertex& vert = verts[u][v];
      vert.p = vec3(0, 0, 0);
      vert.n = vec3(0, 1, 0);
      vert.t = vec2(u/(float)prec, v/(float)prec);
      vert.t = vec3(1, 0, 0);
      // interpolate between 4x4 control points
      for (int j = 0, k = 0; j < 4; j++) {
        for (int i = 0; i < 4; i++, k++) {
          vert.p += b[i][u] * b[j][v] * controlPoints[k];
        }
      }
    }
  }
  // Create triangles from the vertex grid created
  std::vector<Vertex> meshVerts{};
  meshVerts.reserve(prec*prec*6); // 2*prec*prec triangles
  for (int i = 0; i < prec; i++) {
    for (int j = 0; j < prec; j++) {
      // for each "square" in the grid, create 2 traingle
      // first triangle
      meshVerts.push_back(verts[i  ][j  ]);
      meshVerts.push_back(verts[i+1][j  ]);
      meshVerts.push_back(verts[i+1][j+1]);
      // second triangle
      meshVerts.push_back(verts[i  ][j  ]);
      meshVerts.push_back(verts[i+1][j+1]);
      meshVerts.push_back(verts[i  ][j+1]);
    }
  }
  return Mesh(meshVerts);
}