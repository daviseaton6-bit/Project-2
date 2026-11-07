#include <iostream>
#include <vector>
#include <string>
#include <fstream>

#include "FacilityManager.h"
#include "CampusGraph.h"

using namespace std;

vector<string> loadBuildings(const string& filename)
{
    vector<string> buildings;

    ifstream file(filename);

    if (!file)
    {
        cout << "Error opening "
             << filename << endl;
        return buildings;
    }

    string line;

    while (getline(file, line))
    {
        if (line.empty())
            continue;

        if (line[0] == '#')
            continue;

        buildings.push_back(line);
    }

    return buildings;
}

int main()
{
    FacilityManager facilityManager;
    CampusGraph campusGraph;
    
string graphFolder = ".";
    campusGraph.loadBuildings(graphFolder);
    campusGraph.loadConnections(graphFolder);
    
    vector<string> buildings =
        loadBuildings("buildings.txt");

facilityManager.loadFacilities(
    "facilities.txt",
    buildings);

    int choice;

    do
    {
        cout << "\n===== MENU =====\n";
        cout << "1. View all facilities\n";
        cout << "2. Search facility by ID\n";
        cout << "3. Search facility by name\n";
        cout << "4. Search facility by category\n";
        cout << "5. Category report\n";
        cout << "6. View campus graph\n";
        cout << "7. View building counts and degrees\n";
        cout << "8. View reachable buildings\n";
        cout << "0. Exit\n";

        cout << "\nChoice: ";
        cin >> choice;
        cin.ignore();

        switch (choice)
        {
            case 1:
                facilityManager.displayFacilities();
                break;

            case 2:
            {
                string id;
                cout << "Enter ID: ";
                getline(cin, id);

                facilityManager.searchByID(id);
                break;
            }

            case 3:
            {
                string name;
                cout << "Enter Name: ";
                getline(cin, name);

                facilityManager.searchByName(name);
                break;
            }

            case 4:
            {
                string category;
                cout << "Enter Category: ";
                getline(cin, category);

                facilityManager.searchByCategory(category);
                break;
            }

            case 5:
                facilityManager.categoryReport();
                break;
            
             case 6:
                campusGraph.displayGraph();
                break;

            case 7:
                campusGraph.displayCountsAndDegrees();
                break;

            case 8:
                campusGraph.reachableMenu();
                break;
        }

    } while (choice != 0);

    return 0;
}
