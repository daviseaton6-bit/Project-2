#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <vector>
#include <string>
#include "Facility.h"

class HashTable
{
private:
    static const int CAPACITY = 101;

    std::vector<Facility*> table[CAPACITY];

    int entries;
    int collisions;

    int hashFunction(const std::string& key);

public:
    HashTable();

    bool insert(Facility* facility);

    Facility* search(const std::string& id);

    int getEntries() const;
    int getCollisions() const;
    int getCapacity() const;
};

#endif