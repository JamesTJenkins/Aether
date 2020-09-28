#pragma once
#include <glm/glm.hpp>
#include <string>

namespace Shaders {

	class Material {
	public:
		Material();

		std::string name;
		glm::vec3 diffuseColor;
		std::string diffuseTexture;
		glm::vec3 specularColor;
		std::string specularTexture;
		std::string specularHighlight;
		float specularExponent;
		float opticalDensity;
		float dissolve;
		int illumination;
		glm::vec3 ambientColor;
		std::string ambientTexture;
		std::string alphaMask;
		std::string bumpMap;
	};

}