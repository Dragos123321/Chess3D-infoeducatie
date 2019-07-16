#include "light.h"

Light::Light()
{
    m_lightPos = glm::vec3(100.0, -100.0, 0.0);
    m_diffuseColor = glm::vec3(1.0, 1.0, 1.0);
    m_specColor = glm::vec3(1.0, 1.0, 1.0);
}
