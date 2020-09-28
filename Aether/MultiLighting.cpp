#include "MultiLighting.h"
#include "Entity.h"

namespace Shaders {

	MultiLighting::MultiLighting(const std::string& filename) : Shader(filename, Shader::VERTEX | Shader::FRAGMENT) {
		glBindAttribLocation(program, POSITION_A, "Position");
		glBindAttribLocation(program, NORMAL_A, "Normal");
	}

	void MultiLighting::Update(const Camera& camera, const Material& material, Objects::GameObject* go) {
		// Shader and material
		Bind();
		
		//SetMat4("transform", camera.WorldToCameraMatrix());
		SetMat4("transform", go->transform.LocalToWorldMatrix());
		SetMat4("view", camera.GetViewMatrix());
		SetMat4("projection", camera.GetProjectionMatrix());
		SetVec3("viewPos", camera.transform.position);

		SetVec3("material.diffuse", material.diffuseColor);
		SetVec3("material.specular", material.specularColor);
		SetFloat("material.shininess", material.specularExponent);

		// Lights

        glm::vec3 pointLightPos[] = {
            glm::vec3(0.7f,  0.2f,  2.0f),
            glm::vec3(2.3f, -3.3f, -4.0f),
            glm::vec3(-4.0f,  2.0f, -12.0f),
            glm::vec3(0.0f,  0.0f, -3.0f)
        };

        SetVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
        SetVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
        SetVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
        SetVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
        // point light 1
        SetVec3("pointLights[0].position", pointLightPos[0]);
        SetVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
        SetVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
        SetVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
        SetFloat("pointLights[0].constant", 1.0f);
        SetFloat("pointLights[0].linear", 0.09);
        SetFloat("pointLights[0].quadratic", 0.032);
        // point light 2
        SetVec3("pointLights[1].position", pointLightPos[1]);
        SetVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
        SetVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
        SetVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
        SetFloat("pointLights[1].constant", 1.0f);
        SetFloat("pointLights[1].linear", 0.09);
        SetFloat("pointLights[1].quadratic", 0.032);
        // point light 3
        SetVec3("pointLights[2].position", pointLightPos[2]);
        SetVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
        SetVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
        SetVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
        SetFloat("pointLights[2].constant", 1.0f);
        SetFloat("pointLights[2].linear", 0.09);
        SetFloat("pointLights[2].quadratic", 0.032);
        // point light 4
        SetVec3("pointLights[3].position", pointLightPos[3]);
        SetVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
        SetVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
        SetVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
        SetFloat("pointLights[3].constant", 1.0f);
        SetFloat("pointLights[3].linear", 0.09);
        SetFloat("pointLights[3].quadratic", 0.032);
        // spotLight
        SetVec3("spotLight.position", camera.transform.position);
        SetVec3("spotLight.direction", camera.transform.Forward());
        SetVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
        SetVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
        SetVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
        SetFloat("spotLight.constant", 1.0f);
        SetFloat("spotLight.linear", 0.09);
        SetFloat("spotLight.quadratic", 0.032);
        SetFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        SetFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));
	}
}