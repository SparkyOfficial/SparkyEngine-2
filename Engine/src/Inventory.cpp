#include "../include/Inventory.h"
#include "../include/FileUtils.h"
#include "../include/Logger.h"
#include <fstream>
#include <sstream>

namespace Sparky {

    // Item implementation
    Item::Item(const std::string& name, int maxStack) : 
        name(name), maxStack(maxStack), quantity(1) {
    }

    Item::~Item() {
    }

    void Item::setQuantity(int quantity) {
        this->quantity = quantity;
    }

    void Item::addQuantity(int amount) {
        this->quantity += amount;
        if (this->quantity > maxStack) {
            this->quantity = maxStack;
        }
    }

    void Item::removeQuantity(int amount) {
        this->quantity -= amount;
        if (this->quantity < 0) {
            this->quantity = 0;
        }
    }

    bool Item::isStackable() const {
        return maxStack > 1;
    }

    std::unique_ptr<Item> Item::clone() const {
        return std::make_unique<Item>(name, maxStack);
    }

    // Inventory implementation
    Inventory::Inventory(int size) : size(size) {
        items.reserve(size);
        for (int i = 0; i < size; i++) {
            items.push_back(nullptr);
        }
        SPARKY_LOG_DEBUG("Created inventory with size: " + std::to_string(size));
    }

    Inventory::~Inventory() {
    }

    bool Inventory::addItem(std::unique_ptr<Item> item) {
        if (!item) return false;

        // Check if item is stackable
        if (item->isStackable()) {
            // Try to stack with existing items
            for (auto& slot : items) {
                if (slot && slot->getName() == item->getName() && 
                    slot->getQuantity() < slot->getMaxStack()) {
                    int availableSpace = slot->getMaxStack() - slot->getQuantity();
                    int transferAmount = std::min(availableSpace, item->getQuantity());
                    
                    slot->addQuantity(transferAmount);
                    item->removeQuantity(transferAmount);
                    
                    if (item->getQuantity() <= 0) {
                        return true; // All items were stacked
                    }
                }
            }
        }

        // Add to first empty slot
        for (size_t i = 0; i < items.size(); i++) {
            if (!items[i]) {
                items[i] = std::move(item);
                SPARKY_LOG_DEBUG("Item added to inventory slot: " + std::to_string(i));
                return true;
            }
        }

        SPARKY_LOG_WARNING("Failed to add item to inventory - inventory full");
        return false; // Inventory full
    }

    bool Inventory::removeItem(const std::string& itemName, int quantity) {
        for (auto& slot : items) {
            if (slot && slot->getName() == itemName) {
                if (slot->getQuantity() >= quantity) {
                    slot->removeQuantity(quantity);
                    if (slot->getQuantity() <= 0) {
                        slot.reset();
                    }
                    SPARKY_LOG_DEBUG("Removed " + std::to_string(quantity) + " " + itemName + " from inventory");
                    return true;
                }
            }
        }
        
        SPARKY_LOG_WARNING("Failed to remove item from inventory: " + itemName);
        return false;
    }

    Item* Inventory::getItem(const std::string& itemName) {
        for (auto& slot : items) {
            if (slot && slot->getName() == itemName) {
                return slot.get();
            }
        }
        return nullptr;
    }

    Item* Inventory::getItemAt(int index) {
        if (index >= 0 && index < static_cast<int>(items.size()) && items[index]) {
            return items[index].get();
        }
        return nullptr;
    }

    bool Inventory::stackItem(Item* item) {
        if (!item || !item->isStackable()) return false;

        for (auto& slot : items) {
            if (slot && slot->getName() == item->getName() && 
                slot->getQuantity() < slot->getMaxStack()) {
                int availableSpace = slot->getMaxStack() - slot->getQuantity();
                int transferAmount = std::min(availableSpace, item->getQuantity());
                
                slot->addQuantity(transferAmount);
                item->removeQuantity(transferAmount);
                
                if (item->getQuantity() <= 0) {
                    return true; // All items were stacked
                }
            }
        }
        
        return false;
    }

    int Inventory::getItemCount() const {
        int count = 0;
        for (const auto& slot : items) {
            if (slot) {
                count++;
            }
        }
        return count;
    }

    bool Inventory::isFull() const {
        return getItemCount() >= size;
    }

    bool Inventory::isEmpty() const {
        return getItemCount() == 0;
    }

    bool Inventory::saveToFile(const std::string& filepath) {
        SPARKY_LOG_DEBUG("Saving inventory to file: " + filepath);
        
        try {
            std::ofstream file(filepath);
            if (!file.is_open()) {
                SPARKY_LOG_ERROR("Failed to open inventory file for writing: " + filepath);
                return false;
            }

            file << size << "\n";
            
            for (size_t i = 0; i < items.size(); i++) {
                if (items[i]) {
                    file << i << " " << items[i]->getName() << " " << items[i]->getQuantity() << " " << items[i]->getMaxStack() << "\n";
                }
            }
            
            file.close();
            SPARKY_LOG_DEBUG("Inventory saved successfully");
            return true;
        } catch (const std::exception& e) {
            SPARKY_LOG_ERROR("Failed to save inventory: " + std::string(e.what()));
            return false;
        }
    }

    bool Inventory::loadFromFile(const std::string& filepath) {
        SPARKY_LOG_DEBUG("Loading inventory from file: " + filepath);
        
        try {
            if (!FileUtils::fileExists(filepath)) {
                SPARKY_LOG_WARNING("Inventory file does not exist: " + filepath);
                return false;
            }

            std::vector<char> data = FileUtils::readFile(filepath);
            std::string content(data.begin(), data.end());
            
            std::istringstream iss(content);
            std::string line;
            
            // Read size
            if (!std::getline(iss, line)) {
                SPARKY_LOG_ERROR("Invalid inventory file format");
                return false;
            }
            
            int loadedSize = std::stoi(line);
            if (loadedSize != size) {
                SPARKY_LOG_WARNING("Inventory size mismatch, resizing");
                items.clear();
                size = loadedSize;
                items.reserve(size);
                for (int i = 0; i < size; i++) {
                    items.push_back(nullptr);
                }
            }
            
            // Read items
            while (std::getline(iss, line)) {
                std::istringstream lineStream(line);
                int index;
                std::string name;
                int quantity, maxStack;
                
                if (lineStream >> index >> name >> quantity >> maxStack) {
                    if (index >= 0 && index < size) {
                        auto item = std::make_unique<Item>(name, maxStack);
                        item->setQuantity(quantity);
                        items[index] = std::move(item);
                    }
                }
            }
            
            SPARKY_LOG_DEBUG("Inventory loaded successfully");
            return true;
        } catch (const std::exception& e) {
            SPARKY_LOG_ERROR("Failed to load inventory: " + std::string(e.what()));
            return false;
        }
    }
}