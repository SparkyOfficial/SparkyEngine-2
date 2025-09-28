#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <cmath>

// Simple 3D vector class
struct Vector3 {
    float x, y, z;
    
    Vector3() : x(0), y(0), z(0) {}
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
    
    Vector3 operator+(const Vector3& other) const {
        return Vector3(x + other.x, y + other.y, z + other.z);
    }
    
    Vector3 operator-(const Vector3& other) const {
        return Vector3(x - other.x, y - other.y, z - other.z);
    }
    
    Vector3 operator*(float scalar) const {
        return Vector3(x * scalar, y * scalar, z * scalar);
    }
    
    float length() const {
        return std::sqrt(x*x + y*y + z*z);
    }
};

// Base class for all game objects
class GameObject {
protected:
    Vector3 position;
    Vector3 size;
    std::string name;
    
public:
    GameObject(const std::string& name) : name(name), position(0, 0, 0), size(1, 1, 1) {}
    
    virtual ~GameObject() = default;
    
    void setPosition(const Vector3& pos) { position = pos; }
    Vector3 getPosition() const { return position; }
    
    void setSize(const Vector3& s) { size = s; }
    Vector3 getSize() const { return size; }
    
    std::string getName() const { return name; }
    
    virtual void update(float deltaTime) {}
};

// Player class
class Player : public GameObject {
private:
    Vector3 velocity;
    bool isGrounded;
    float jumpForce;
    
public:
    Player() : GameObject("Player"), velocity(0, 0, 0), isGrounded(true), jumpForce(10.0f) {}
    
    void moveForward(float deltaTime) {
        position.z -= 5.0f * deltaTime;
    }
    
    void moveBackward(float deltaTime) {
        position.z += 5.0f * deltaTime;
    }
    
    void moveLeft(float deltaTime) {
        position.x -= 5.0f * deltaTime;
    }
    
    void moveRight(float deltaTime) {
        position.x += 5.0f * deltaTime;
    }
    
    void jump() {
        if (isGrounded) {
            velocity.y = jumpForce;
            isGrounded = false;
        }
    }
    
    void update(float deltaTime) override {
        // Apply gravity
        if (!isGrounded) {
            velocity.y -= 20.0f * deltaTime;
        }
        
        // Update position based on velocity
        position = position + velocity * deltaTime;
        
        // Simple ground collision
        if (position.y <= 0) {
            position.y = 0;
            velocity.y = 0;
            isGrounded = true;
        }
    }
    
    bool getIsGrounded() const { return isGrounded; }
};

// Platform class
class Platform : public GameObject {
public:
    Platform(const std::string& name) : GameObject(name) {}
};

// Gun class
class Gun : public GameObject {
private:
    int ammo;
    
public:
    Gun() : GameObject("Gun"), ammo(30) {}
    
    void shoot() {
        if (ammo > 0) {
            std::cout << "Bang! Ammo remaining: " << --ammo << std::endl;
        } else {
            std::cout << "Click! Out of ammo." << std::endl;
        }
    }
    
    void reload() {
        ammo = 30;
        std::cout << "Gun reloaded. Ammo: " << ammo << std::endl;
    }
};

// Simple game class
class SimpleGame {
private:
    std::unique_ptr<Player> player;
    std::unique_ptr<Gun> gun;
    std::vector<std::unique_ptr<Platform>> platforms;
    bool gameRunning;
    
public:
    SimpleGame() : gameRunning(true) {
        // Create player
        player = std::make_unique<Player>();
        player->setPosition(Vector3(0, 0, 0));
        
        // Create gun
        gun = std::make_unique<Gun>();
        
        // Create platforms
        createLevel();
    }
    
    void createLevel() {
        // Create a floor
        auto floor = std::make_unique<Platform>("Floor");
        floor->setPosition(Vector3(0, -1, 0));
        floor->setSize(Vector3(20, 1, 20));
        platforms.push_back(std::move(floor));
        
        // Create some platforms
        auto platform1 = std::make_unique<Platform>("Platform1");
        platform1->setPosition(Vector3(5, 2, 0));
        platform1->setSize(Vector3(3, 1, 3));
        platforms.push_back(std::move(platform1));
        
        auto platform2 = std::make_unique<Platform>("Platform2");
        platform2->setPosition(Vector3(-5, 4, 0));
        platform2->setSize(Vector3(3, 1, 3));
        platforms.push_back(std::move(platform2));
        
        // Create a ramp
        auto ramp = std::make_unique<Platform>("Ramp");
        ramp->setPosition(Vector3(0, 1, 5));
        ramp->setSize(Vector3(8, 1, 3));
        platforms.push_back(std::move(ramp));
        
        // Create stairs
        for (int i = 0; i < 5; i++) {
            auto step = std::make_unique<Platform>("Step" + std::to_string(i));
            step->setPosition(Vector3(8, 0.5f + i * 1.0f, -5 + i * 1.0f));
            step->setSize(Vector3(2, 1, 1));
            platforms.push_back(std::move(step));
        }
    }
    
    void update(float deltaTime) {
        // Update player
        player->update(deltaTime);
    }
    
    void handleInput(char input) {
        switch (input) {
            case 'w':
            case 'W':
                player->moveForward(0.1f);
                break;
            case 's':
            case 'S':
                player->moveBackward(0.1f);
                break;
            case 'a':
            case 'A':
                player->moveLeft(0.1f);
                break;
            case 'd':
            case 'D':
                player->moveRight(0.1f);
                break;
            case ' ':
                player->jump();
                break;
            case 'f':
            case 'F':
                gun->shoot();
                break;
            case 'r':
            case 'R':
                gun->reload();
                break;
            case 'q':
            case 'Q':
                gameRunning = false;
                break;
        }
    }
    
    void render() {
        // Simple text-based rendering
        Vector3 pos = player->getPosition();
        std::cout << "\n=== Sparky Engine Platformer ===" << std::endl;
        std::cout << "Player Position: (" << pos.x << ", " << pos.y << ", " << pos.z << ")" << std::endl;
        std::cout << "Player Grounded: " << (player->getIsGrounded() ? "Yes" : "No") << std::endl;
        std::cout << "Platforms: " << platforms.size() << std::endl;
        std::cout << "\nControls:" << std::endl;
        std::cout << "  WASD - Move" << std::endl;
        std::cout << "  Space - Jump" << std::endl;
        std::cout << "  F - Shoot" << std::endl;
        std::cout << "  R - Reload" << std::endl;
        std::cout << "  Q - Quit" << std::endl;
        std::cout << "===============================" << std::endl;
    }
    
    bool isRunning() const {
        return gameRunning;
    }
};

int main() {
    std::cout << "Sparky Engine Platformer Game" << std::endl;
    std::cout << "============================" << std::endl;
    
    SimpleGame game;
    
    // Simple game loop
    while (game.isRunning()) {
        game.render();
        game.update(0.1f);
        
        std::cout << "\nEnter command: ";
        char input;
        std::cin >> input;
        
        game.handleInput(input);
    }
    
    std::cout << "Thanks for playing!" << std::endl;
    return 0;
}