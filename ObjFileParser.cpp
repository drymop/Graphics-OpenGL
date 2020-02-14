#include "ObjFileParser.h"

#include <fstream>
#include <sstream>
#include <stdexcept>


Mesh parseObjFile(const std::string& _filename) {
  std::ifstream ifs(_filename);
  if(!ifs) {
    throw std::invalid_argument("Cannot open file");
  }

  std::vector<glm::vec3> positions;
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
        vertices.emplace_back(positions[p-1], normals[n-1]);
      }
    }
  }

  return Mesh(vertices);
}

Material parseMaterialFile(const std::string& _filename) {
  std::ifstream ifs(_filename);
  if(!ifs) {
    throw std::invalid_argument("Cannot open file");
  }

  Material m{};
  std::string line;
  
  while(ifs) {
    getline(ifs, line);
    std::istringstream iss(line);
    std::string tag;
    iss >> tag;
    if(tag == "Ka") {
      iss >> m.ka.x >> m.ka.y >> m.ka.z;
    }
    else if (tag == "Kd") {
      iss >> m.kd.x >> m.kd.y >> m.kd.z;
    } 
    else if (tag == "Ks") {
      iss >> m.ks.x >> m.ks.y >> m.ks.z;
    }
    else if (tag == "Kr") {
      iss >> m.ks.x >> m.ks.y >> m.ks.z;
    }
    else if (tag == "Ns") {
      iss >> m.shininess;
    }
  }
  return m;
}
