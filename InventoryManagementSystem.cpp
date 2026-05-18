manager.addItem(Item(id, name, quantity, price, reorderLevel));
std::cout << "Item added with ID " << id << ".\n";
}

void searchItems(const InventoryManager& manager) {
    std::cout << "\nSearch Items\n";
    std::cout << "1. Search by ID\n";
    std::cout << "2. Search by name\n";

    int choice = readInt("Choose an option: ", 1);
    if (choice == 1) {
        int id = readInt("Item ID: ");
        const Item* item = manager.findById(id);
        if (item == nullptr) {
            std::cout << "No item found with that ID.\n";
        }
        else {
            printItemTable({ *item });
        }
    }
    else if (choice == 2) {
        std::string name = readText("Name contains: ");
        printItemTable(manager.findByName(name));
    }
    else {
        std::cout << "Invalid search option.\n";
    }
}

void updateItem(InventoryManager& manager) {
    std::cout << "\nUpdate Item\n";
    int id = readInt("Item ID: ");

    Item* existing = manager.findById(id);
    if (existing == nullptr) {
        std::cout << "No item found with that ID.\n";
        return;
    }

    std::cout << "Current item:\n";
    printItemTable({ *existing });

    std::string name = readText("New name: ");
    int quantity = readInt("New quantity: ");
    double price = readDouble("New unit price: ");
    int reorderLevel = readInt("New reorder level: ");

    manager.updateItem(id, name, quantity, price, reorderLevel);
    std::cout << "Item updated.\n";
}

void deleteItem(InventoryManager& manager) {
    std::cout << "\nDelete Item\n";
    int id = readInt("Item ID: ");

    if (manager.removeItem(id)) {
        std::cout << "Item deleted.\n";
    }
    else {
        std::cout << "No item found with that ID.\n";
    }
}

void sortInventory(InventoryManager& manager) {
    std::cout << "\nSort Inventory\n";
    std::cout << "1. By name\n";
    std::cout << "2. By quantity\n";
    std::cout << "3. By total value\n";

    int choice = readInt("Choose an option: ", 1);
    if (choice == 1) {
        manager.sortByName();
        std::cout << "Sorted by name.\n";
    }
    else if (choice == 2) {
        manager.sortByQuantity();
        std::cout << "Sorted by quantity.\n";
    }
    else if (choice == 3) {
        manager.sortByTotalValue();
        std::cout << "Sorted by total value.\n";
    }
    else {
        std::cout << "Invalid sort option.\n";
    }
}

void showReports(const InventoryManager& manager) {
    std::cout << "\nReports\n";
    std::cout << "Total inventory value: $" << std::fixed << std::setprecision(2)
        << manager.getInventoryValue() << "\n\n";

    std::cout << "Low stock items:\n";
    printItemTable(manager.getLowStockItems());
}

void showMenu() {
    std::cout << "\nInventory Management System\n";
    std::cout << "1. View all items\n";
    std::cout << "2. Add item\n";
    std::cout << "3. Search items\n";
    std::cout << "4. Update item\n";
    std::cout << "5. Delete item\n";
    std::cout << "6. Sort inventory\n";
    std::cout << "7. Reports\n";
    std::cout << "8. Save\n";
    std::cout << "9. Exit\n";
}
}

int main() {
    InventoryManager manager;

    if (manager.loadFromFile(DATA_FILE)) {
        std::cout << "Loaded inventory from " << DATA_FILE << ".\n";
    }
    else {
        std::cout << "No existing data file found. Starting with an empty inventory.\n";
    }

    bool running = true;
    while (running) {
        showMenu();
        int choice = readInt("Choose an option: ", 1);

        switch (choice) {
        case 1:
            printItemTable(manager.getItems());
            break;
        case 2:
            addItem(manager);
            break;
        case 3:
            searchItems(manager);
            break;
        case 4:
            updateItem(manager);
            break;
        case 5:
            deleteItem(manager);
            break;
        case 6:
            sortInventory(manager);
            break;
        case 7:
            showReports(manager);
            break;
        case 8:
            if (manager.saveToFile(DATA_FILE)) {
                std::cout << "Inventory saved.\n";
            }
            else {
                std::cout << "Could not save inventory file.\n";
            }
            break;
        case 9:
            if (manager.saveToFile(DATA_FILE)) {
                std::cout << "Inventory saved. Goodbye.\n";
            }
            else {
                std::cout << "Could not save inventory file. Goodbye.\n";
            }
            running = false;
            break;
        default:
            std::cout << "Invalid menu option.\n";
            break;
        }
    }

    return 0;
}