#ifndef MATERIAL_H_
#define MATERIAL_H_

class Material 
{
  public:
    Material(glm::vec3 _ka, glm::vec3 _kd, glm::vec3 _ks, float _shininess)
      : m_ka(_ka), m_kd(_kd), m_ks(_ks), m_shininess(_shininess)
    {}

    glm::vec3 getKa() {
      return m_ka;
    }

    glm::vec3 getKd() {
      return m_kd;
    }

    glm::vec3 getKs() {
      return m_ks;
    }

    float getShininess() {
      return m_shininess;
    }

  private:
    glm::vec3 m_ka;
    glm::vec3 m_kd;
    glm::vec3 m_ks;
    float m_shininess;
};


#endif // MATERIAL_H_