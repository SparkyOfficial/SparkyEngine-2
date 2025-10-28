#include "../include/Logger.h"
#include <iostream>

using namespace Sparky;

// Simple demo to test additional abstraction layers
int main() {
    std::cout << "=== Additional Abstraction Layers Demo ===" << std::endl;
    
    // Initialize logger
    Logger& logger = Logger::getInstance();
    logger.setLogLevel(LogLevel::DEBUG);
    
    std::cout << "Abstraction layer features:" << std::endl;
    std::cout << "  Shader Program abstraction:" << std::endl;
    std::cout << "    - Vertex/fragment shader compilation" << std::endl;
    std::cout << "    - Shader linking and validation" << std::endl;
    std::cout << "    - Uniform management" << std::endl;
    std::cout << "    - Shader resource binding" << std::endl;
    
    std::cout << "  Texture abstraction:" << std::endl;
    std::cout << "    - Image loading and format conversion" << std::endl;
    std::cout << "    - Mipmap generation" << std::endl;
    std::cout << "    - Texture filtering and wrapping" << std::endl;
    std::cout << "    - Memory management" << std::endl;
    
    std::cout << "  Material abstraction:" << std::endl;
    std::cout << "    - PBR material properties" << std::endl;
    std::cout << "    - Descriptor set management" << std::endl;
    std::cout << "    - Shader and texture binding" << std::endl;
    std::cout << "    - Material instance sharing" << std::endl;
    
    // Show abstraction layer benefits
    std::cout << "\nAbstraction layer benefits:" << std::endl;
    std::cout << "  1. Renderer independence - can switch between Vulkan, OpenGL, DirectX" << std::endl;
    std::cout << "  2. Resource management - automatic cleanup and memory management" << std::endl;
    std::cout << "  3. Error handling - graceful degradation when features aren't available" << std::endl;
    std::cout << "  4. Performance optimization - batching, instancing, and caching" << std::endl;
    std::cout << "  5. Debugging support - detailed logging and validation" << std::endl;
    
    std::cout << "=== Demo Complete ===" << std::endl;
    
    return 0;
}