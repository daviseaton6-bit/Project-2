#ifndef FACILITYMANAGER_H
#define FACILITYMANAGER_H

#include <vector>
#include <string>

#include "Facility.h"
#include "HashTable.h"

class FacilityManager
{
private:
    std::vector<Facility> facilities;
    HashTable facilityTable;

public:



    // Loading
    bool loadFacilities(
        const std::string& filename,
        const std::vector<std::string>& buildings);

    // Display
    void displayFacilities() const;

    // Searches
    void searchByID(const std::string& id);

    void searchByName(
        const std::string& name);

    void searchByCategory(
        const std::string& category);

    // Report
    void categoryReport();

    // Utility Functions
    std::string trim(std::string str);

    std::string toUpper(std::string str);

    bool validID(std::string id);
};

#endif