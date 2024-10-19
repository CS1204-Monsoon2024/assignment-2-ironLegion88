// HashTable.cpp
#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

class HashTable {
private:
    vector<int> table;   // Hash table storage
    vector<bool> tombstone;  // Tracks deleted slots (tombstone markers)
    int currentSize;      // Number of elements in the table
    int capacity;         // Current capacity of the table
    static constexpr double LOAD_FACTOR = 0.8;  // Load factor threshold for resizing

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
        capacity = nextPrime(2 * oldCapacity);  // New capacity must be prime
        vector<int> oldTable = table;
        vector<bool> oldTombstone = tombstone;

        table.assign(capacity, -1);  // Reset the new table
        tombstone.assign(capacity, false);
        currentSize = 0;

        for (int i = 0; i < oldCapacity; ++i) {
            if (oldTable[i] != -1 && !oldTombstone[i]) {
                insert(oldTable[i]);  // Reinsert elements into the new table
            }
        }
    }

public:
    // Constructor to initialize the hash table
    HashTable(int size = 7) {
        capacity = nextPrime(size);  // Ensure initial size is prime
        table.assign(capacity, -1);
        tombstone.assign(capacity, false);
        currentSize = 0;
    }

    // Insert function with quadratic probing
    void insert(int key) {
        int index = hash(key);
        int i = 0;

        // Quadratic probing to find an open slot
        while (true) {
            int newIndex = (index + i * i) % capacity;

            if (table[newIndex] == -1) {
                // Insert key and update size
                table[newIndex] = key;
                tombstone[newIndex] = false;
                ++currentSize;

                // Check if rehashing is needed
                if ((double)currentSize / capacity >= LOAD_FACTOR) {
                    rehash();
                }
                return;
            } else if (table[newIndex] == key) {
                cout << "Duplicate key insertion is not allowed" << endl;
                return;
            }
            ++i;

            if (i == capacity) {
                cout << "Max probing limit reached!" << endl;
                return;
            }
        }
    }

    // Remove function with quadratic probing
    void remove(int key) {
        int index = hash(key);
        int i = 0;

        // Quadratic probing to find the key
        while (true) {
            int newIndex = (index + i * i) % capacity;

            if (table[newIndex] == key) {
                // Mark as tombstone
                table[newIndex] = -1;
                tombstone[newIndex] = true;
                --currentSize;
                return;
            } else if (table[newIndex] == -1 && !tombstone[newIndex]) {
                cout << "Element not found" << endl;
                return;
            }
            ++i;

            if (i == capacity) {
                cout << "Element not found" << endl;
                return;
            }
        }
    }

    // Search function with quadratic probing
    int search(int key) const {
        int index = hash(key);
        int i = 0;

        // Quadratic probing to search for the key
        while (true) {
            int newIndex = (index + i * i) % capacity;

            if (table[newIndex] == key) {
                return newIndex;  // Return the index of the key
            } else if (table[newIndex] == -1 && !tombstone[newIndex]) {
                return -1;  // Key not found
            }
            ++i;

            if (i == capacity) {
                return -1;  // Key not found
            }
        }
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
