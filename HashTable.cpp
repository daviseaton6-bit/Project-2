#include "HashTable.h"

HashTable::HashTable()
{
    entries = 0;
    collisions = 0;
}

int HashTable::hashFunction(const std::string& key)
{
    int hash = 0;

    for (char c : key)
    {
        hash = (hash * 31 + c) % CAPACITY;
    }

    return hash;
}

bool HashTable::insert(Facility* facility)
{
    int index = hashFunction(facility->id);

    for (Facility* existing : table[index])
    {
        if (existing->id == facility->id)
        {
            return false;
        }
    }

    if (!table[index].empty())
    {
        collisions++;
    }

    table[index].push_back(facility);
    entries++;

    return true;
}

Facility* HashTable::search(const std::string& id)
{
    int index = hashFunction(id);

    for (Facility* facility : table[index])
    {
        if (facility->id == id)
        {
            return facility;
        }
    }

    return nullptr;
}

int HashTable::getEntries() const
{
    return entries;
}

int HashTable::getCollisions() const
{
    return collisions;
}

int HashTable::getCapacity() const
{
    return CAPACITY;
}