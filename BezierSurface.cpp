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
  // also precompute the corresponding derivative dB_i/dt
  float dB[4][prec+1];
  for (int i = 0; i <= prec; i++) {
    float t = i / (float)prec;
    float t2 = t * t;
    float t3 = t2 * t;
    float s = 1-t;
    float s2 = s * s;
    float s3 = s2 * s;
    b[0][i] = s3;
    b[1][i] = 3*t*s2;
    b[2][i] = 3*t2*s;
    b[3][i] = t3;
    dB[0][i] = -3*s2;
    dB[1][i] = 3*s2 - 6*t*s;
    dB[2][i] = 6*t*s - 3*t2;
    dB[3][i] = 3*t2;
  }
  // Divide the surface into a grid of (prec+1)*(prec+1) points
  Vertex verts[prec+1][prec+1];
  for(int u = 0; u <= prec; u++) {
    for (int v = 0; v <= prec; v++) {
      // compute the point (u, v)
      Vertex& vert = verts[u][v];
      vert.p = vec3(0, 0, 0);
      vert.t = vec2(u/(float)prec, v/(float)prec);
      vert.tg = vec3(0, 0, 0);
      vec3 biTangent = vec3(0, 0, 0);
      // interpolate between 4x4 control points
      for (int j = 0, k = 0; j < 4; j++) {
        for (int i = 0; i < 4; i++, k++) {
          vert.p  += b[i][u] * b[j][v] * controlPoints[k];
          // tangent is derivative with respect to u
          vert.tg += dB[i][u] * b[j][v] * controlPoints[k];
          // tangent is derivative with respect to v
          biTangent += b[i][u] * dB[j][v] * controlPoints[k];
        }
      }
      // calculate vertex normal from tange
      vert.n = glm::normalize(glm::cross(vert.tg, biTangent));
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