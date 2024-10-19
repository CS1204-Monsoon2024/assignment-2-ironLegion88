// HashTable.cpp
#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

class HashTable {
private:
    vector<int> table;   // Hash table storage
    int currentSize;      // Number of elements in the table
    int capacity;         // Current capacity of the table
    static constexpr double LOAD_FACTOR = 0.8;  // Static constant to avoid const-related issues

    // Helper function to find the next prime number >= 'num'
    int nextPrime(int num) {
        while (!isPrime(num)) ++num;
        return num;
    }

    // Helper function to check if a number is prime
    bool isPrime(int num) {
        if (num < 2) return false;
        for (int i = 2; i <= sqrt(num); ++i) {
            if (num % i == 0) return false;
        }
        return true;
    }

    // Hash function: h(k) = k % m
    int hash(int key) const {
        return key % capacity;
    }

    // Function to rehash all elements when resizing
    void rehash() {
        int oldCapacity = capacity;
        capacity = nextPrime(2 * oldCapacity);
        vector<int> oldTable = table;
        table.assign(capacity, -1);  // Reset the new table
        currentSize = 0;

        for (int key : oldTable) {
            if (key != -1) {
                insert(key);  // Reinsert elements into the new table
            }
        }
    }

public:
    // Constructor to initialize the hash table
    HashTable(int size = 7) {
        capacity = nextPrime(size);  // Ensure initial size is prime
        table.assign(capacity, -1);
        currentSize = 0;
    }

    // Insert function with quadratic probing
    void insert(int key) {
        int index = hash(key);
        int i = 0;

        // Quadratic probing to find an open slot
        while (table[(index + i * i) % capacity] != -1) {
            if (table[(index + i * i) % capacity] == key) {
                cout << "Duplicate key insertion is not allowed" << endl;
                return;
            }
            ++i;
            if (i == capacity) {
                cout << "Max probing limit reached!" << endl;
                return;
            }
        }

        // Insert key and update size
        table[(index + i * i) % capacity] = key;
        ++currentSize;

        // Check if rehashing is needed
        if ((double)currentSize / capacity >= LOAD_FACTOR) {
            rehash();
        }
    }

    // Remove function with quadratic probing
    void remove(int key) {
        int index = hash(key);
        int i = 0;

        // Quadratic probing to find the key
        while (table[(index + i * i) % capacity] != key) {
            if (table[(index + i * i) % capacity] == -1) {
                cout << "Element not found" << endl;
                return;
            }
            ++i;
            if (i == capacity) {
                cout << "Element not found" << endl;
                return;
            }
        }

        // Remove key and mark slot as -1
        table[(index + i * i) % capacity] = -1;
        --currentSize;
    }

    // Search function with quadratic probing
    int search(int key) const {
        int index = hash(key);
        int i = 0;

        // Quadratic probing to search for the key
        while (table[(index + i * i) % capacity] != key) {
            if (table[(index + i * i) % capacity] == -1) {
                return -1;  // Key not found
            }
            ++i;
            if (i == capacity) {
                return -1;  // Key not found
            }
        }
        return (index + i * i) % capacity;  // Return the index of the key
    }

    // Print the entire hash table
    void printTable() const {
        for (int i = 0; i < capacity; ++i) {
            if (table[i] == -1) {
                cout << "- ";
            } else {
                cout << table[i] << " ";
            }
        }
        cout << endl;
    }
};
