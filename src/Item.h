#ifndef ITEM_H
#define ITEM_H

#include <string>

class Item {
public:
    Item();
    Item(int id, std::string name, int quantity, double price, int reorderLevel);

    int getId() const;
    const std::string& getName() const;
    int getQuantity() const;
    double getPrice() const;
    int getReorderLevel() const;

    void setName(const std::string& name);
    void setQuantity(int quantity);
    void setPrice(double price);
    void setReorderLevel(int reorderLevel);

    double getTotalValue() const;
    bool isLowStock() const;

private:
    int id;
    std::string name;
    int quantity;
    double price;
    int reorderLevel;
};

#endif
