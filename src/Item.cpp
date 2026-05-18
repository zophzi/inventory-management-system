#include "Item.h"

#include <utility>

Item::Item()
    : id(0), name(""), quantity(0), price(0.0), reorderLevel(0) {
}

Item::Item(int id, std::string name, int quantity, double price, int reorderLevel)
    : id(id),
      name(std::move(name)),
      quantity(quantity),
      price(price),
      reorderLevel(reorderLevel) {
}

int Item::getId() const {
    return id;
}

const std::string& Item::getName() const {
    return name;
}

int Item::getQuantity() const {
    return quantity;
}

double Item::getPrice() const {
    return price;
}

int Item::getReorderLevel() const {
    return reorderLevel;
}

void Item::setName(const std::string& newName) {
    name = newName;
}

void Item::setQuantity(int newQuantity) {
    quantity = newQuantity;
}

void Item::setPrice(double newPrice) {
    price = newPrice;
}

void Item::setReorderLevel(int newReorderLevel) {
    reorderLevel = newReorderLevel;
}

double Item::getTotalValue() const {
    return quantity * price;
}

bool Item::isLowStock() const {
    return quantity <= reorderLevel;
}
