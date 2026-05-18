#include "InventoryManager.h"

#include <algorithm>
#include <cctype>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <system_error>

namespace {
std::string toLower(std::string text) {
    std::transform(text.begin(), text.end(), text.begin(), [](unsigned char ch) {
        return static_cast<char>(std::tolower(ch));
    });
    return text;
}

std::string escapeCsv(const std::string& value) {
    bool needsQuotes = value.find_first_of(",\"") != std::string::npos;
    if (!needsQuotes) {
        return value;
    }

    std::string escaped = "\"";
    for (char ch : value) {
        if (ch == '"') {
            escaped += "\"\"";
        } else {
            escaped += ch;
        }
    }
    escaped += "\"";
    return escaped;
}

std::vector<std::string> parseCsvLine(const std::string& line) {
    std::vector<std::string> fields;
    std::string current;
    bool inQuotes = false;

    for (std::size_t i = 0; i < line.size(); ++i) {
        char ch = line[i];

        if (ch == '"' && inQuotes && i + 1 < line.size() && line[i + 1] == '"') {
            current += '"';
            ++i;
        } else if (ch == '"') {
            inQuotes = !inQuotes;
        } else if (ch == ',' && !inQuotes) {
            fields.push_back(current);
            current.clear();
        } else {
            current += ch;
        }
    }

    fields.push_back(current);
    return fields;
}
}

bool InventoryManager::loadFromFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file) {
        return false;
    }

    std::vector<Item> loadedItems;
    std::string line;

    std::getline(file, line);
    while (std::getline(file, line)) {
        if (line.empty()) {
            continue;
        }

        std::vector<std::string> fields = parseCsvLine(line);
        if (fields.size() != 5) {
            continue;
        }

        try {
            int id = std::stoi(fields[0]);
            std::string name = fields[1];
            int quantity = std::stoi(fields[2]);
            double price = std::stod(fields[3]);
            int reorderLevel = std::stoi(fields[4]);
            loadedItems.emplace_back(id, name, quantity, price, reorderLevel);
        } catch (...) {
            continue;
        }
    }

    items = loadedItems;
    return true;
}

bool InventoryManager::saveToFile(const std::string& filePath) const {
    std::filesystem::path path(filePath);
    if (path.has_parent_path()) {
        std::error_code error;
        std::filesystem::create_directories(path.parent_path(), error);
    }

    std::ofstream file(filePath);
    if (!file) {
        return false;
    }

    file << "id,name,quantity,price,reorder_level\n";
    for (const Item& item : items) {
        file << item.getId() << ','
             << escapeCsv(item.getName()) << ','
             << item.getQuantity() << ','
             << item.getPrice() << ','
             << item.getReorderLevel() << '\n';
    }

    return true;
}

bool InventoryManager::addItem(const Item& item) {
    if (findById(item.getId()) != nullptr) {
        return false;
    }

    items.push_back(item);
    return true;
}

bool InventoryManager::updateItem(int id, const std::string& name, int quantity, double price, int reorderLevel) {
    Item* item = findById(id);
    if (item == nullptr) {
        return false;
    }

    item->setName(name);
    item->setQuantity(quantity);
    item->setPrice(price);
    item->setReorderLevel(reorderLevel);
    return true;
}

bool InventoryManager::removeItem(int id) {
    auto itemToRemove = std::remove_if(items.begin(), items.end(), [id](const Item& item) {
        return item.getId() == id;
    });

    if (itemToRemove == items.end()) {
        return false;
    }

    items.erase(itemToRemove, items.end());
    return true;
}

Item* InventoryManager::findById(int id) {
    auto found = std::find_if(items.begin(), items.end(), [id](const Item& item) {
        return item.getId() == id;
    });

    return found == items.end() ? nullptr : &(*found);
}

const Item* InventoryManager::findById(int id) const {
    auto found = std::find_if(items.begin(), items.end(), [id](const Item& item) {
        return item.getId() == id;
    });

    return found == items.end() ? nullptr : &(*found);
}

std::vector<Item> InventoryManager::findByName(const std::string& name) const {
    std::vector<Item> results;
    std::string needle = toLower(name);

    for (const Item& item : items) {
        if (toLower(item.getName()).find(needle) != std::string::npos) {
            results.push_back(item);
        }
    }

    return results;
}

void InventoryManager::sortByName() {
    std::sort(items.begin(), items.end(), [](const Item& left, const Item& right) {
        return toLower(left.getName()) < toLower(right.getName());
    });
}

void InventoryManager::sortByQuantity() {
    std::sort(items.begin(), items.end(), [](const Item& left, const Item& right) {
        return left.getQuantity() < right.getQuantity();
    });
}

void InventoryManager::sortByTotalValue() {
    std::sort(items.begin(), items.end(), [](const Item& left, const Item& right) {
        return left.getTotalValue() > right.getTotalValue();
    });
}

int InventoryManager::getNextId() const {
    int highestId = 0;
    for (const Item& item : items) {
        highestId = std::max(highestId, item.getId());
    }
    return highestId + 1;
}

double InventoryManager::getInventoryValue() const {
    double total = 0.0;
    for (const Item& item : items) {
        total += item.getTotalValue();
    }
    return total;
}

std::vector<Item> InventoryManager::getLowStockItems() const {
    std::vector<Item> lowStockItems;
    for (const Item& item : items) {
        if (item.isLowStock()) {
            lowStockItems.push_back(item);
        }
    }
    return lowStockItems;
}

const std::vector<Item>& InventoryManager::getItems() const {
    return items;
}
