#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

class HashTable {
private:
    vector<int> table;
    int currentSize;
    int capacity;
    const double LOAD_FACTOR = 0.8;

    int nextPrime(int num) {
        while (!isPrime(num)) ++num;
        return num;
    }

    bool isPrime(int num) {
        if (num < 2) return false;
        for (int i = 2; i <= sqrt(num); ++i) {
            if (num % i == 0) return false;
        }
        return true;
    }

    int hash(int key) const {
        return key % capacity;
    }

    void rehash() {
        int oldCapacity = capacity;
        capacity = nextPrime(2 * oldCapacity);
        vector<int> oldTable = table;
        table.assign(capacity, -1);
        currentSize = 0;

        for (int key : oldTable) {
            if (key != -1) {
                insert(key);
            }
        }
    }

public:
    HashTable(int size = 7) {
        capacity = nextPrime(size);
        table.assign(capacity, -1);
        currentSize = 0;
    }

    void insert(int key) {
        int index = hash(key);
        int i = 0;

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

        table[(index + i * i) % capacity] = key;
        ++currentSize;

        if ((double)currentSize / capacity >= LOAD_FACTOR) {
            rehash();
        }
    }

    void remove(int key) {
        int index = hash(key);
        int i = 0;

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

        table[(index + i * i) % capacity] = -1;
        --currentSize;
    }

    int search(int key) const {
        int index = hash(key);
        int i = 0;

        while (table[(index + i * i) % capacity] != key) {
            if (table[(index + i * i) % capacity] == -1) {
                return -1;
            }
            ++i;
            if (i == capacity) {
                return -1;
            }
        }
        return (index + i * i) % capacity;
    }

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
