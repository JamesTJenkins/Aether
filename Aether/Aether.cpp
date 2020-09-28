#include <iostream>
#include <glm/gtc/constants.hpp>
#include <glm/glm.hpp>
#include "Window.h"
#include "Camera.h"
#include "Events.h"
#include "Logic.h"
#include "AssetImporter.h"

int main() {
    std::cout << "Aether Engine Initalizing...\n";

    Window window;

    Meshes::AssetImporter ai;
    
    Camera camera(Objects::Transform (glm::vec3(0, 0, -5)), 1280.0f / 720.0f, glm::half_pi<float>(), 0.03f, 1000.0f);

    // Logic
    EngineSystems::EventHandler eh;
    EngineSystems::Logic logic(&eh);

    ai.LoadInitList("Res/Init/GameObjectInit.xml", logic);

    while (window.IsOpen()) {
        // INPUTS
        window.HandleEvents();

        // LOGICS
        logic.UpdateLogic();
        eh.HandleEvents();

        // RENDERING
        window.Clear(0.0f, 0.2f, 0.4f, 1.0f);
        
        for (size_t i = 0; i < logic.GetEntityCount(); i++) {
            logic.gameEntities[i].Render(camera);
        }
        
        window.SwapBuffer();
    }
}
