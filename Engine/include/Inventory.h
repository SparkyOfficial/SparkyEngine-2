#pragma once

#include <vector>
#include <string>
#include <memory>

namespace Sparky {
    class Item {
    public:
        Item(const std::string& name, int maxStack = 1);
        virtual ~Item();

        const std::string& getName() const { return name; }
        int getMaxStack() const { return maxStack; }
        int getQuantity() const { return quantity; }

        void setQuantity(int quantity);
        void addQuantity(int amount);
        void removeQuantity(int amount);

        virtual bool isStackable() const;
        virtual std::unique_ptr<Item> clone() const;

    protected:
        std::string name;
        int maxStack;
        int quantity;
    };

    class Inventory {
    public:
        Inventory(int size = 20);
        ~Inventory();

        // Item management
        bool addItem(std::unique_ptr<Item> item);
        bool removeItem(const std::string& itemName, int quantity = 1);
        Item* getItem(const std::string& itemName);
        Item* getItemAt(int index);

        // Stack management
        bool stackItem(Item* item);

        // Inventory info
        int getSize() const { return size; }
        int getItemCount() const;
        bool isFull() const;
        bool isEmpty() const;

        // Serialization
        bool saveToFile(const std::string& filepath);
        bool loadFromFile(const std::string& filepath);

    private:
        int size;
        std::vector<std::unique_ptr<Item>> items;
    };
}