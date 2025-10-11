#include "PBRExample.h"
#include <iostream>

int main() {
    Sparky::PBRExample pbrExample;
    
    std::cout << "Initializing PBR Example..." << std::endl;
    
    if (!pbrExample.initialize()) {
        std::cerr << "Failed to initialize PBR example!" << std::endl;
        return -1;
    }
    
    std::cout << "Starting PBR rendering demo..." << std::endl;
    std::cout << "Controls:" << std::endl;
    std::cout << "  WASD - Move camera" << std::endl;
    std::cout << "  Right Mouse Button + Drag - Look around" << std::endl;
    std::cout << "  ESC - Exit" << std::endl;
    
    pbrExample.run();
    
    return 0;
}