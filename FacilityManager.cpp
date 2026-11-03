#include "FacilityManager.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cctype>

using namespace std;


//Helper Functions


string FacilityManager::trim(string str)
{
    while (!str.empty() &&
           isspace(static_cast<unsigned char>(str.front())))
    {
        str.erase(str.begin());
    }

    while (!str.empty() &&
           isspace(static_cast<unsigned char>(str.back())))
    {
        str.pop_back();
    }

    return str;
}

string FacilityManager::toUpper(string str)
{
    for (char &c : str)
    {
        c = toupper(static_cast<unsigned char>(c));
    }

    return str;
}

bool FacilityManager::validID(string id)
{
    if (id.length() != 4)
        return false;

    if (id[0] != 'F')
        return false;

    for (int i = 1; i < 4; i++)
    {
        if (!isdigit(id[i]))
        {
            return false;
        }
    }

    return true;
}


//Load Facilities


bool FacilityManager::loadFacilities(
    const string& filename,
    const vector<string>& buildings)
{
    ifstream file(filename);

    int accepted = 0;
    int rejected = 0;

    if (!file)
    {
        cout << "Error opening file: "
             << filename << endl;
        return false;
    }

    string line;
    int lineNumber = 0;

    while (getline(file, line))
    {
        lineNumber++;

        line = trim(line);

        if (line.empty())
            continue;

        if (line[0] == '#')
            continue;

        stringstream ss(line);

        string id;
        string name;
        string building;
        string category;

        if (!getline(ss, id, '|') ||
            !getline(ss, name, '|') ||
            !getline(ss, building, '|') ||
            !getline(ss, category))
        {
            cout << "Warning: "
                 << filename
                 << " line "
                 << lineNumber
                 << " invalid format"
                 << endl;

            rejected++;
            continue;
        }

        id = toUpper(trim(id));
        name = trim(name);
        building = trim(building);
        category = trim(category);

        if (!validID(id))
        {
            cout << "Warning: "
                 << filename
                 << " line "
                 << lineNumber
                 << " invalid ID."
                 << endl;

            rejected++;
            continue;
        }

        if (name.empty() ||
            building.empty() ||
            category.empty())
        {
            cout << "Warning: "
                 << filename
                 << " line "
                 << lineNumber
                 << " empty field."
                 << endl;

            rejected++;
            continue;
        }

        bool buildingFound = false;

        for (const string& b : buildings)
        {
            if (toUpper(b) ==
                toUpper(building))
            {
                building = b;
                buildingFound = true;
                break;
            }
        }

        if (!buildingFound)
        {
            cout << "Warning: "
                 << filename
                 << " line "
                 << lineNumber
                 << " unknown building."
                 << endl;

            rejected++;
            continue;
        }

        Facility facility;

        facility.id = id;
        facility.name = name;
        facility.building = building;
        facility.category = category;

        facilities.push_back(facility);

        Facility* ptr =
            &facilities.back();

        if (!facilityTable.insert(ptr))
        {
            facilities.pop_back();

            cout << "Warning: "
                 << filename
                 << " line "
                 << lineNumber
                 << " duplicate ID."
                 << endl;

            rejected++;
            continue;
        }

        accepted++;
    }

    cout << "\nFacilities Loaded\n";
    cout << "Accepted: "
         << accepted << endl;

    cout << "Rejected: "
         << rejected << endl;

    cout << "Hash Capacity: "
         << facilityTable.getCapacity()
         << endl;

    cout << "Entries: "
         << facilityTable.getEntries()
         << endl;

    cout << "Collisions: "
         << facilityTable.getCollisions()
         << endl;

    return true;
}


//Display All Facilities


void FacilityManager::displayFacilities() const
{
    if (facilities.empty())
    {
        cout << "No facilities loaded."
             << endl;
        return;
    }

    for (const auto& f : facilities)
    {
        cout << f.id
             << " | "
             << f.name
             << " | "
             << f.building
             << " | "
             << f.category
             << endl;
    }

    cout << "\nTotal Facilities: "
         << facilities.size()
         << endl;
}


//Search By ID


void FacilityManager::searchByID(
    const std::string& input)
{
    std::string id =
        toUpper(trim(input));

    if (!validID(id))
    {
        std::cout
            << "Invalid format.\n";
        return;
    }

    // TEMP TEST
    for (const auto& f : facilities)
    {
        if (f.id == id)
        {
            std::cout << "\nFound:\n";

            std::cout
                << f.id << " | "
                << f.name << " | "
                << f.building << " | "
                << f.category
                << std::endl;

            return;
        }
    }

    std::cout
        << "Not found: "
        << id
        << std::endl;
}


//Search By Name


void FacilityManager::searchByName(
    const string& input)
{
    string target =
        toUpper(trim(input));

    int matches = 0;

    for (const auto& f : facilities)
    {
        if (toUpper(f.name) == target)
        {
            cout << f.id
                 << " | "
                 << f.name
                 << " | "
                 << f.building
                 << " | "
                 << f.category
                 << endl;

            matches++;
        }
    }

    cout << "\nMatches: "
         << matches
         << endl;
}



//Search By Category


void FacilityManager::searchByCategory(
    const string& input)
{
    string target =
        toUpper(trim(input));

    int matches = 0;

    for (const auto& f : facilities)
    {
        if (toUpper(f.category)
            == target)
        {
            cout << f.id
                 << " | "
                 << f.name
                 << " | "
                 << f.building
                 << " | "
                 << f.category
                 << endl;

            matches++;
        }
    }

    cout << "\nMatches: "
         << matches
         << endl;
}

/*
--------------------------------------------------
Category Report
--------------------------------------------------
*/

void FacilityManager::categoryReport()
{
    if (facilities.empty())
    {
        cout << "No facilities available."
             << endl;

        cout << "Total: 0"
             << endl;

        return;
    }

    vector<string> categories;
    vector<int> counts;

    for (const auto& f : facilities)
    {
        bool found = false;

        for (size_t i = 0;
             i < categories.size();
             i++)
        {
            if (toUpper(categories[i])
                ==
                toUpper(f.category))
            {
                counts[i]++;
                found = true;
                break;
            }
        }

        if (!found)
        {
            categories.push_back(
                f.category);

            counts.push_back(1);
        }
    }

    cout << "\nFacility Category Report\n\n";

    for (size_t i = 0;
         i < categories.size();
         i++)
    {
        double percent =
            (counts[i] * 100.0)
            / facilities.size();

        cout << left
             << setw(25)
             << categories[i]

             << setw(8)
             << counts[i]

             << fixed
             << setprecision(1)
             << percent
             << "%"
             << endl;
    }

    cout << "\nTotal: "
         << facilities.size()
         << endl;
}