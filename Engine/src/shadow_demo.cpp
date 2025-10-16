#include "../include/SparkyEngine.h"
#include "../include/VulkanRenderer.h"
#include "../include/PBRRenderer.h"
#include "../include/ShadowMap.h"
#include "../include/Camera.h"
#include "../include/Mesh.h"
#include "../include/PBRMaterial.h"
#include "../include/DirectionalLight.h"
#include <iostream>
#include <memory>

#ifdef HAS_GLFW
#include <GLFW/glfw3.h>
#endif

using namespace Sparky;

class ShadowDemo {
public:
    ShadowDemo() 
        : m_engine(nullptr)
        , m_window(nullptr)
        , m_running(false) {
    }

    bool initialize() {
#ifdef HAS_GLFW
        // Initialize GLFW
        if (!glfwInit()) {
            std::cerr << "Failed to initialize GLFW" << std::endl;
            return false;
        }

        // Create window
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        m_window = glfwCreateWindow(1280, 720, "Shadow Mapping Demo", nullptr, nullptr);
        if (!m_window) {
            std::cerr << "Failed to create window" << std::endl;
            glfwTerminate();
            return false;
        }

        // Create engine
        m_engine = std::make_unique<SparkyEngine>();
        
        // Initialize engine with window handle
        if (!m_engine->initialize(glfwGetWin32Window(m_window))) {
            std::cerr << "Failed to initialize engine" << std::endl;
            return false;
        }

        // Get renderer
        m_renderer = &m_engine->getRenderer();
        
        // Create PBR renderer
        m_pbrRenderer = std::make_unique<PBRRenderer>();
        if (!m_pbrRenderer->initialize(m_renderer)) {
            std::cerr << "Failed to initialize PBR renderer" << std::endl;
            return false;
        }
        
        // Create shadow map
        m_shadowMap = std::make_unique<ShadowMap>();
        if (!m_shadowMap->initialize(m_renderer, 2048, 2048)) {
            std::cerr << "Failed to initialize shadow map" << std::endl;
            return false;
        }
        
        // Set shadow map in PBR renderer
        m_pbrRenderer->setShadowMap(m_shadowMap.get());
        
        // Setup scene
        setupScene();
        
        m_running = true;
        return true;
#else
        std::cerr << "GLFW not available, cannot run shadow demo" << std::endl;
        return false;
#endif
    }

    void run() {
#ifdef HAS_GLFW
        while (m_running && !glfwWindowShouldClose(m_window)) {
            glfwPollEvents();
            
            // Update
            update();
            
            // Render
            render();
        }
#endif
    }

    void shutdown() {
#ifdef HAS_GLFW
        m_shadowMap.reset();
        m_pbrRenderer.reset();
        m_engine.reset();
        
        if (m_window) {
            glfwDestroyWindow(m_window);
        }
        glfwTerminate();
#endif
    }

private:
    void setupScene() {
        // Setup camera
        m_camera.setPosition(glm::vec3(0.0f, 2.0f, 5.0f));
        m_camera.lookAt(glm::vec3(0.0f, 0.0f, 0.0f));
        
        // Setup directional light
        m_light.setDirection(glm::vec3(-0.5f, -1.0f, -0.5f));
        m_light.setColor(glm::vec3(1.0f, 1.0f, 1.0f));
        m_light.setIntensity(1.0f);
        
        // Set light in PBR renderer
        m_pbrRenderer->setDirectionalLight(m_light);
        
        // Set light direction in shadow map
        m_shadowMap->setLightDirection(m_light.getDirection());
        
        // Create a simple plane mesh and material for the ground
        // In a real implementation, this would load actual mesh data
        // For now, we'll just create placeholders
        
        // Create a simple cube mesh and material for an object
        // In a real implementation, this would load actual mesh data
        // For now, we'll just create placeholders
    }

    void update() {
        // Handle input
#ifdef HAS_GLFW
        if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            m_running = false;
        }
#endif
        
        // Update camera
        // In a real implementation, this would handle user input for camera movement
        // For now, we'll just keep the camera static
    }

    void render() {
        // Begin scene
        m_pbrRenderer->beginScene(m_camera);
        
        // Submit objects for rendering
        // In a real implementation, this would submit actual meshes with materials
        // For now, we'll just call endScene to trigger rendering
        
        // End scene and render
        m_pbrRenderer->endScene();
        
        // Present to screen
        m_renderer->render();
    }

private:
    std::unique_ptr<SparkyEngine> m_engine;
    VulkanRenderer* m_renderer;
    std::unique_ptr<PBRRenderer> m_pbrRenderer;
    std::unique_ptr<ShadowMap> m_shadowMap;
    Camera m_camera;
    DirectionalLight m_light;
    
#ifdef HAS_GLFW
    GLFWwindow* m_window;
#endif
    bool m_running;
};

int main() {
    ShadowDemo demo;
    
    if (!demo.initialize()) {
        std::cerr << "Failed to initialize shadow demo" << std::endl;
        return -1;
    }
    
    std::cout << "Shadow Mapping Demo initialized successfully" << std::endl;
    std::cout << "Press ESC to exit" << std::endl;
    
    demo.run();
    demo.shutdown();
    
    return 0;
}