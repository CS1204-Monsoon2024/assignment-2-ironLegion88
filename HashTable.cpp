#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

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

// Helper function to get the next prime greater than or equal to 'n'
int nextPrime(int n) {
    while (!isPrime(n)) ++n;
    return n;
}

class HashTable {
private:
    vector<int> table;
    vector<bool> occupied;  // Track if a position was ever occupied
    int size;               // Current size of the table
    int count;              // Current number of elements in the table
    const double loadFactorThreshold = 0.8;

    // Hash function: key % size
    int hash(int key) const {
        return key % size;
    }

    // Resize the hash table to the next prime size >= 2 * current size
    void resize() {
        int newSize = nextPrime(2 * size);
        vector<int> newTable(newSize, -1);
        vector<bool> newOccupied(newSize, false);

        // Rehash all existing keys to the new table
        for (int i = 0; i < size; ++i) {
            if (occupied[i] && table[i] != -1) {
                int newIndex = table[i] % newSize;
                int j = 0;

                // Quadratic probing to insert existing elements
                while (newOccupied[(newIndex + j * j) % newSize]) {
                    ++j;
                }
                newTable[(newIndex + j * j) % newSize] = table[i];
                newOccupied[(newIndex + j * j) % newSize] = true;
            }
        }

        table = std::move(newTable);
        occupied = std::move(newOccupied);
        size = newSize;
    }

public:
    // Constructor to initialize the hash table with a given size
    HashTable(int initialSize) {
        size = nextPrime(initialSize);
        table.resize(size, -1);
        occupied.resize(size, false);
        count = 0;
    }

    // Insert a key into the hash table
    void insert(int key) {
        if (search(key) != -1) {
            cout << "Duplicate key insertion is not allowed" << endl;
            return;
        }

        if (count >= loadFactorThreshold * size) {
            resize();
        }

        int index = hash(key);
        int i = 0;

        // Quadratic probing to find the correct position
        while (occupied[(index + i * i) % size]) {
            ++i;
            if (i > size / 2) {  // Max probing limit reached
                cout << "Max probing limit reached!" << endl;
                return;
            }
        }

        table[(index + i * i) % size] = key;
        occupied[(index + i * i) % size] = true;
        ++count;
    }

    // Remove a key from the hash table
    void remove(int key) {
        int index = search(key);
        if (index == -1) {
            cout << "Element not found" << endl;
            return;
        }
        table[index] = -1;
        --count;
    }

    // Search for a key and return its index; return -1 if not found
    int search(int key) const {
        int index = hash(key);
        int i = 0;

        // Quadratic probing to find the key
        while (occupied[(index + i * i) % size]) {
            int probeIndex = (index + i * i) % size;
            if (table[probeIndex] == key) {
                return probeIndex;
            }
            ++i;
            if (i > size / 2) break;  // Max probing limit
        }
        return -1;  // Key not found
    }

    // Print the current state of the hash table
    void printTable() const {
        for (int i = 0; i < size; ++i) {
            if (table[i] == -1) {
                cout << "- ";
            } else {
                cout << table[i] << " ";
            }
        }
        cout << endl;
    }
};
