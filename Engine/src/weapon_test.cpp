#include "../include/WeaponSystem.h"
#include "../include/Inventory.h"
#include "../include/WeaponItem.h"
#include "../include/AmmoItem.h"
#include "../include/Gun.h"
#include "../include/Logger.h"
#include <memory>

int main() {
    Sparky::Logger::getInstance().setLogLevel(Sparky::LogLevel::DEBUG);
    
    Sparky::Logger::getInstance().info("=== Weapon Switching and Inventory System Test ===");
    
    // Create inventory
    auto inventory = std::make_unique<Sparky::Inventory>(20);
    Sparky::Logger::getInstance().info("Created inventory with size: " + std::to_string(inventory->getSize()));
    
    // Create some weapons
    Sparky::Logger::getInstance().info("Creating weapons...");
    
    auto pistol = std::make_unique<Sparky::Gun>();
    pistol->setWeaponType("Pistol");
    pistol->setAmmo(12);
    pistol->setMagazineSize(12);
    pistol->setTotalAmmo(120);
    pistol->setDamage(25.0f);
    
    auto rifle = std::make_unique<Sparky::Gun>();
    rifle->setWeaponType("Rifle");
    rifle->setAmmo(30);
    rifle->setMagazineSize(30);
    rifle->setTotalAmmo(210);
    rifle->setDamage(35.0f);
    
    auto shotgun = std::make_unique<Sparky::Gun>();
    shotgun->setWeaponType("Shotgun");
    shotgun->setAmmo(8);
    shotgun->setMagazineSize(8);
    shotgun->setTotalAmmo(40);
    shotgun->setDamage(50.0f);
    
    // Create weapon items
    auto pistolItem = std::make_unique<Sparky::WeaponItem>("Pistol", std::move(pistol));
    auto rifleItem = std::make_unique<Sparky::WeaponItem>("Rifle", std::move(rifle));
    auto shotgunItem = std::make_unique<Sparky::WeaponItem>("Shotgun", std::move(shotgun));
    
    // Add weapons to inventory
    Sparky::Logger::getInstance().info("Adding weapons to inventory...");
    inventory->addItem(std::move(pistolItem));
    inventory->addItem(std::move(rifleItem));
    inventory->addItem(std::move(shotgunItem));
    
    // Create ammo items
    Sparky::Logger::getInstance().info("Creating ammo...");
    
    auto pistolAmmo = std::make_unique<Sparky::AmmoItem>("Pistol Ammo", Sparky::AmmoType::REGULAR, 60, 100);
    auto rifleAmmo = std::make_unique<Sparky::AmmoItem>("Rifle Ammo", Sparky::AmmoType::ARMOR_PIERCING, 90, 100);
    auto shotgunAmmo = std::make_unique<Sparky::AmmoItem>("Shotgun Shells", Sparky::AmmoType::EXPLOSIVE, 24, 50);
    
    // Add ammo to inventory
    Sparky::Logger::getInstance().info("Adding ammo to inventory...");
    inventory->addItem(std::move(pistolAmmo));
    inventory->addItem(std::move(rifleAmmo));
    inventory->addItem(std::move(shotgunAmmo));
    
    // Show inventory contents
    Sparky::Logger::getInstance().info("Inventory contents:");
    for (int i = 0; i < inventory->getSize(); i++) {
        Sparky::Item* item = inventory->getItemAt(i);
        if (item) {
            Sparky::Logger::getInstance().info("  Slot " + std::to_string(i) + ": " + item->getName() + 
                                             " (Qty: " + std::to_string(item->getQuantity()) + ")");
        }
    }
    
    // Test weapon switching system
    Sparky::Logger::getInstance().info("Testing weapon switching system...");
    
    // Create weapon system (without player for this test)
    Sparky::WeaponSystem weaponSystem(nullptr);
    weaponSystem.setInventory(inventory.get());
    
    // Add weapon slots
    weaponSystem.addWeaponSlot(0); // Pistol
    weaponSystem.addWeaponSlot(1); // Rifle
    weaponSystem.addWeaponSlot(2); // Shotgun
    
    // Test equipping weapons
    Sparky::Logger::getInstance().info("Equipping pistol...");
    weaponSystem.equipWeapon(0);
    
    if (weaponSystem.hasWeaponEquipped()) {
        Sparky::Gun* currentWeapon = weaponSystem.getCurrentWeapon();
        Sparky::Logger::getInstance().info("Current weapon: " + std::string(currentWeapon->getWeaponType()));
        Sparky::Logger::getInstance().info("Ammo: " + std::to_string(weaponSystem.getMagazineAmmo()) + 
                                         "/" + std::to_string(weaponSystem.getTotalAmmo()));
    }
    
    // Test switching weapons
    Sparky::Logger::getInstance().info("Switching to rifle...");
    weaponSystem.switchToNextWeapon();
    
    if (weaponSystem.hasWeaponEquipped()) {
        Sparky::Gun* currentWeapon = weaponSystem.getCurrentWeapon();
        Sparky::Logger::getInstance().info("Current weapon: " + std::string(currentWeapon->getWeaponType()));
        Sparky::Logger::getInstance().info("Ammo: " + std::to_string(weaponSystem.getMagazineAmmo()) + 
                                         "/" + std::to_string(weaponSystem.getTotalAmmo()));
    }
    
    // Test quick slots
    Sparky::Logger::getInstance().info("Setting up quick slots...");
    weaponSystem.setQuickSlot(1, 0); // Slot 1 = Pistol
    weaponSystem.setQuickSlot(2, 1); // Slot 2 = Rifle
    weaponSystem.setQuickSlot(3, 2); // Slot 3 = Shotgun
    
    Sparky::Logger::getInstance().info("Using quick slot 3 (Shotgun)...");
    weaponSystem.useQuickSlot(3);
    
    if (weaponSystem.hasWeaponEquipped()) {
        Sparky::Gun* currentWeapon = weaponSystem.getCurrentWeapon();
        Sparky::Logger::getInstance().info("Current weapon: " + std::string(currentWeapon->getWeaponType()));
    }
    
    Sparky::Logger::getInstance().info("");
    Sparky::Logger::getInstance().info("=== Weapon System Features ===");
    Sparky::Logger::getInstance().info("1. Multiple weapon support");
    Sparky::Logger::getInstance().info("2. Ammo management");
    Sparky::Logger::getInstance().info("3. Weapon switching (sequential and quick slots)");
    Sparky::Logger::getInstance().info("4. Inventory integration");
    Sparky::Logger::getInstance().info("5. Weapon statistics tracking");
    Sparky::Logger::getInstance().info("6. Ammo type variations");
    
    Sparky::Logger::getInstance().info("");
    Sparky::Logger::getInstance().info("Weapon switching and inventory system test completed successfully!");
    return 0;
}