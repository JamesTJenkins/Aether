#include "Material.h"

namespace Shaders {

	Material::Material() : name("name"), diffuseColor(glm::vec3(0.0f, 0.0f, 0.0f)), diffuseTexture(""), specularColor(1.0f, 1.0f, 1.0f), specularTexture(""), specularHighlight(""), specularExponent(32.0), opticalDensity(0.0), dissolve(0.0), illumination(0), ambientColor(glm::vec3(1.0f, 1.0f, 1.0f)), ambientTexture(""), alphaMask(""), bumpMap("") {

	}
}