#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

class HashTable {
private:
    vector<int> table;    // Hash table storage
    int currentSize;       // Number of elements in the table
    int capacity;          // Current capacity of the table
    static constexpr double LOAD_FACTOR = 0.8;  // Load factor threshold
    const int EMPTY = -1;  // Represents empty slots
    const int TOMBSTONE = -2;  // Represents deleted slots

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
        capacity = nextPrime(2 * oldCapacity);  // New prime capacity
        vector<int> oldTable = table;
        table.assign(capacity, EMPTY);  // Reset the new table with EMPTY
        currentSize = 0;  // Reset size before reinserting elements

        // Reinsert all valid elements from the old table into the new table
        for (int key : oldTable) {
            if (key != EMPTY && key != TOMBSTONE) {
                insert(key);  // Use insert to apply new capacity's hash
            }
        }
    }

public:
    // Constructor to initialize the hash table
    HashTable(int size = 7) {
        capacity = nextPrime(size);  // Ensure initial size is prime
        table.assign(capacity, EMPTY);
        currentSize = 0;
    }

    // Insert function with quadratic probing
    void insert(int key) {
        int index = hash(key);
        int i = 0;
        int tombstoneIndex = -1;  // Track a possible tombstone slot

        // Quadratic probing to find an open slot or tombstone
        while (table[(index + i * i) % capacity] != EMPTY) {
            int currentIndex = (index + i * i) % capacity;

            if (table[currentIndex] == key) {
                cout << "Duplicate key insertion is not allowed" << endl;
                return;
            }

            // If we encounter a tombstone, store its position for reuse
            if (table[currentIndex] == TOMBSTONE && tombstoneIndex == -1) {
                tombstoneIndex = currentIndex;
            }

            ++i;
            if (i == capacity) {
                cout << "Max probing limit reached!" << endl;
                return;
            }
        }

        // Insert key in either the found tombstone slot or the empty slot
        if (tombstoneIndex != -1) {
            table[tombstoneIndex] = key;
        } else {
            table[(index + i * i) % capacity] = key;
        }

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
            if (table[(index + i * i) % capacity] == EMPTY) {
                cout << "Element not found" << endl;
                return;
            }
            ++i;
            if (i == capacity) {
                cout << "Element not found" << endl;
                return;
            }
        }

        // Mark slot as tombstone
        table[(index + i * i) % capacity] = TOMBSTONE;
        --currentSize;
    }

    // Search function with quadratic probing
    int search(int key) const {
        int index = hash(key);
        int i = 0;

        // Quadratic probing to search for the key
        while (table[(index + i * i) % capacity] != key) {
            if (table[(index + i * i) % capacity] == EMPTY) {
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
            if (table[i] == EMPTY) {
                cout << "- ";
            } else if (table[i] == TOMBSTONE) {
                cout << "x ";  // Mark tombstone for clarity
            } else {
                cout << table[i] << " ";
            }
        }
        cout << endl;
    }
};

int main() {
    HashTable ht(5);

    ht.insert(1);
    ht.insert(4);
    ht.insert(17);
    ht.insert(22);  // Causes rehashing
    ht.printTable();

    ht.remove(4);
    ht.printTable();

    cout << "Index of 17: " << ht.search(17) << endl;
    cout << "Index of 4: " << ht.search(4) << endl;  // Should return -1

    return 0;
}
