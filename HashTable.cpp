#include <iostream>
#include <vector>
#include <cmath>

class HashTable {
private:
    struct HashNode {
        int key;
        bool isOccupied;
        bool isDeleted;

        HashNode() : key(-1), isOccupied(false), isDeleted(false) {}
    };

    std::vector<HashNode> table;
    int currentSize;
    int capacity;
    const double LOAD_FACTOR_THRESHOLD = 0.8;

    int hash(int key) const {
        return key % capacity;
    }

    void resize() {
        int newCapacity = getNextPrime(capacity * 2);
        std::vector<HashNode> newTable(newCapacity);
        std::swap(table, newTable);

        int oldCapacity = capacity;
        capacity = newCapacity;
        currentSize = 0;

        for (int i = 0; i < oldCapacity; ++i) {
            if (newTable[i].isOccupied && !newTable[i].isDeleted) {
                insert(newTable[i].key);
            }
        }
    }

    int getNextPrime(int num) {
        while (!isPrime(num)) {
            ++num;
        }
        return num;
    }

    bool isPrime(int num) {
        if (num <= 1) {
            return false;
        }
        for (int i = 2; i <= std::sqrt(num); ++i) {
            if (num % i == 0) {
                return false;
            }
        }
        return true;
    }

public:
    HashTable(int size) : capacity(size), currentSize(0) {
        table.resize(size);
    }

    void insert(int key) {
        if (search(key) != -1) {
            std::cout << "Duplicate key insertion is not allowed" << std::endl;
            return;
        }

        if ((double)currentSize / capacity > LOAD_FACTOR_THRESHOLD) {
            resize();
        }

        int hashIndex = hash(key);
        int i = 0;
        while (i < capacity) {
            int newIndex = (hashIndex + i * i) % capacity;
            if (!table[newIndex].isOccupied || table[newIndex].isDeleted) {
                table[newIndex].key = key;
                table[newIndex].isOccupied = true;
                table[newIndex].isDeleted = false;
                currentSize++;
                return;
            }
            i++;
        }
        std::cout << "Max probing limit reached!" << std::endl;
    }

    void remove(int key) {
        int index = search(key);
        if (index == -1) {
            std::cout << "Element not found" << std::endl;
        } else {
            table[index].isDeleted = true;
            currentSize--;
        }
    }

    int search(int key) {
        int hashIndex = hash(key);
        int i = 0;
        while (i < capacity) {
            int newIndex = (hashIndex + i * i) % capacity;
            if (!table[newIndex].isOccupied) {
                return -1;
            }
            if (table[newIndex].isOccupied && !table[newIndex].isDeleted && table[newIndex].key == key) {
                return newIndex;
            }
            i++;
        }
        return -1;
    }

    void printTable() const {
        for (int i = 0; i < capacity; ++i) {
            if (table[i].isOccupied && !table[i].isDeleted) {
                std::cout << table[i].key << " ";
            } else {
                std::cout << "- ";
            }
        }
        std::cout << std::endl;
    }
};

int main() {
    int initialSize = 7;
    HashTable ht(initialSize);

    ht.insert(1);
    ht.printTable();
    ht.insert(6);
    ht.printTable();
    ht.insert(15);
    ht.printTable();
    ht.insert(25);
    ht.printTable();
    ht.remove(15);
    ht.printTable();
    ht.insert(29);
    ht.printTable();

    int find = ht.search(22);
    std::cout << "Found at: " << find << std::endl;

    return 0;
}
