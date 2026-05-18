#ifndef INVENTORY_MANAGER_H
#define INVENTORY_MANAGER_H

#include "Item.h"

#include <string>
#include <vector>

class InventoryManager {
public:
    bool loadFromFile(const std::string& filePath);
    bool saveToFile(const std::string& filePath) const;

    bool addItem(const Item& item);
    bool updateItem(int id, const std::string& name, int quantity, double price, int reorderLevel);
    bool removeItem(int id);

    Item* findById(int id);
    const Item* findById(int id) const;
    std::vector<Item> findByName(const std::string& name) const;

    void sortByName();
    void sortByQuantity();
    void sortByTotalValue();

    int getNextId() const;
    double getInventoryValue() const;
    std::vector<Item> getLowStockItems() const;
    const std::vector<Item>& getItems() const;

private:
    std::vector<Item> items;
};

#endif
