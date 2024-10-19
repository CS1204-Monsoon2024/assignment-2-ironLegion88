#include <iostream>
#include <vector>
#include <cmath>

// Helper function to check if a number is prime
bool isPrime(int n) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (int i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

// Helper function to find the next prime number greater than or equal to 'n'
int nextPrime(int n) {
    while (!isPrime(n)) ++n;
    return n;
}

class HashTable {
private:
    std::vector<int> table;  // Stores the values in the hash table
    int currentSize;          // Current number of elements in the table
    int capacity;             // Capacity (size) of the hash table
    const double loadFactorThreshold = 0.8;  // Threshold to trigger resizing

    // Hash function: h(k) = k mod m
    int hash(int key) const {
        return key % capacity;
    }

    // Resize the table to a new prime size and rehash existing elements
    void resize() {
        int newCapacity = nextPrime(2 * capacity);  // Get next prime number
        std::vector<int> newTable(newCapacity, -1);  // New empty table
        for (int i = 0; i < capacity; ++i) {
            if (table[i] != -1) {
                rehash(newTable, table[i], newCapacity);  // Rehash elements
            }
        }
        table = std::move(newTable);  // Update table to new resized table
        capacity = newCapacity;
    }

    // Helper function to rehash a key into the new table during resizing
    void rehash(std::vector<int>& newTable, int key, int newCapacity) {
        int index = key % newCapacity;
        int i = 1;
        while (newTable[index] != -1) {
            index = (index + i * i) % newCapacity;
            ++i;
        }
        newTable[index] = key;
    }

public:
    // Constructor to initialize the hash table with a given size
    HashTable(int size) {
        capacity = nextPrime(size);  // Ensure initial size is prime
        table.resize(capacity, -1);  // Initialize with -1 indicating empty slots
        currentSize = 0;
    }

    // Insert a key into the hash table
    void insert(int key) {
        if (search(key) != -1) {
            std::cout << "Duplicate key insertion is not allowed" << std::endl;
            return;
        }
        if ((double)currentSize / capacity >= loadFactorThreshold) {
            resize();  // Resize if load factor exceeds threshold
        }

        int index = hash(key);  // Get the initial hash index
        int i = 1;
        while (table[index] != -1) {  // Quadratic probing to handle collisions
            index = (index + i * i) % capacity;
            if (i > capacity) {
                std::cout << "Max probing limit reached!" << std::endl;
                return;
            }
            ++i;
        }
        table[index] = key;
        ++currentSize;
    }

    // Search for a key and return its index, or -1 if not found
    int search(int key) const {
        int index = hash(key);
        int i = 1;
        while (table[index] != -1) {
            if (table[index] == key) return index;
            index = (index + i * i) % capacity;
            if (i > capacity) break;
            ++i;
        }
        return -1;
    }

    // Remove a key from the hash table
    void remove(int key) {
        int index = search(key);
        if (index == -1) {
            std::cout << "Element not found" << std::endl;
            return;
        }
        table[index] = -1;  // Mark the slot as empty
        --currentSize;
    }

    // Print the current state of the hash table
    void printTable() const {
        for (int i = 0; i < capacity; ++i) {
            if (table[i] == -1)
                std::cout << "- ";
            else
                std::cout << table[i] << " ";
        }
        std::cout << std::endl;
    }
};
