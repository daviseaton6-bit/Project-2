#include <iostream>
#include <vector>
#include <string>
#include <fstream>

#include "FacilityManager.h"
#include "CampusGraph.h"
#include "department_tree.h"

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

    DepartmentTree* deptTree = createTree();
    //loadDepartments(deptTree, "departments.txt"); idk what this is ut its causing issues for me -Aric

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
        cout << "3. View campus graph\n";
        cout << "4. Display graph counts and degrees\n";
        cout << "5. Find reachable buildings (BFS)\n";
        cout << "6. Add department\n";
        cout << "7. Search department\n";
        cout << "8. Display department hierarchy\n";
        cout << "9. Run preorder traversal\n";
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
                campusGraph.displayGraph();
                break;

            case 4:
                campusGraph.displayCountsAndDegrees();
                break;

            case 5:
                campusGraph.reachableMenu();
                break;

            case 6: {
                string name, parent;
                cout << "enter department name: "; getline(cin, name);
                if (deptTree->root) {
                    cout << "enter parent name: "; getline(cin, parent);
                }
                addDepartment(deptTree, name, parent);
cout << "department added successfully" << endl;
                break;
            }
//case seven had an issue i changed it for it work let me compile -aric
            case 7: {
                string name;
                cout << "enter name: "; getline(cin, name);
                DepartmentNode* found = searchDepartment(deptTree->root, name);
                if (found) {
    cout << "name: " << found->name
         << "\nparent: " << (found->parent ? found->parent->name : "none")
         << "\nchildren: " << found->children.size()
         << endl;
} else {
    cout << "not found" << endl;
}
                break;
            }

            case 8:
                displayHierarchy(deptTree->root, 0);
                break;

            case 9: {
                vector<string> result;
                preorderTraversal(deptTree->root, result);
                for (const auto& s : result) cout << s << " ";
                cout << endl;
                break;
            }


            /*
            case 3: // Search facility by name
                // facilityManager.searchByName(name);
                break;
            case 4: // Search facility by category
                // facilityManager.searchByCategory(category);
                break;
            case 5: // Category report
                // facilityManager.categoryReport();
                break;
            */
        }


    } while (choice != 0);

    destroyTree(deptTree->root);
    delete deptTree;

    return 0;
}
