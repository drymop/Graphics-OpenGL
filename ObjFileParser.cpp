#include "ObjFileParser.h"

#include <fstream>
#include <sstream>


Mesh parseObjFile(const std::string& _filename) {
  std::ifstream ifs(_filename);
  if(!ifs) {
    return Mesh();
  }

  std::vector<glm::vec3> positions;
  std::vector<glm::vec2> textures;
  std::vector<glm::vec3> normals;
  std::vector<Vertex> vertices;

  std::string line;
  while(ifs) {
    getline(ifs, line);

    std::istringstream iss(line);
    std::string tag;
    iss >> tag;

    if(tag == "v") {
      glm::vec3 p;
      iss >> p.x >> p.y >> p.z;
      positions.emplace_back(p);
    }
    else if(tag == "vt") {
      glm::vec2 t;
      iss >> t.x >> t.y;
      textures.emplace_back(t);
    }
    else if(tag == "vn") {
      glm::vec3 n;
      iss >> n.x >> n.y >> n.z;
      normals.emplace_back(n);
    }
    else if(tag == "f") {
      for(size_t i = 0; i < 3; ++i) {
        std::string vert;
        iss >> vert;
        size_t p, t, n;
        sscanf(vert.c_str(), "%zu/%zu/%zu", &p, &t, &n);
        vertices.emplace_back(positions[p-1], normals[n-1], textures[t-1]);
      }
    }
  }

  return Mesh(vertices);
}