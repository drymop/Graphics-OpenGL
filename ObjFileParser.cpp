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
  std::vector<glm::vec2> textures;
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
    else if(tag == "vt") {
      glm::vec2 t;
      iss >> t.x >> t.y;
      textures.emplace_back(t);
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

MaterialConfig parseMaterialFile(const std::string& _filename) {
  std::ifstream ifs(_filename);
  if(!ifs) {
    throw std::invalid_argument("Cannot open file");
  }

  MaterialConfig m{};
  m.hasKdMap = m.hasKsMap = false;

  std::string line;
  while(ifs) {
    getline(ifs, line);
    std::istringstream iss(line);
    std::string tag;
    iss >> tag;
    float x,y,z;
    if(tag == "Ka") {
      iss >> x >> y >> z;
      m.defaultMaterial.ka = {x,y,z};
    }
    else if (tag == "Kd") {
      iss >> x >> y >> z;
      m.defaultMaterial.kd = {x,y,z};
    }
    else if (tag == "Ks") {
      iss >> x >> y >> z;
      m.defaultMaterial.ks = {x,y,z};
    }
    else if (tag == "Kr") {
      iss >> x >> y >> z;
      m.defaultMaterial.kr = {x,y,z};
    }
    else if (tag == "Ke") {
      iss >> x >> y >> z;
      m.defaultMaterial.ke = {x,y,z};
    }
    else if (tag == "Ns") {
      iss >> m.defaultMaterial.shininess;
    }
    else if (tag == "map_Kd") {
      iss >> m.kdTextureFile;
      m.kdTextureFile = "models/" + m.kdTextureFile;
      m.hasKdMap = true;
    }
    else if (tag == "map_Ks") {
      iss >> m.ksTextureFile;
      m.ksTextureFile = "models/" + m.ksTextureFile;
      m.hasKsMap = true;
    }
    else if (tag == "map_Ke") {
      iss >> m.keTextureFile;
      m.ksTextureFile = "models/" + m.keTextureFile;
      m.hasKeMap = true;
    }
  }
  return m;
}
