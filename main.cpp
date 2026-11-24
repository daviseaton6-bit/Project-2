#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

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
    loadDepartments(deptTree, "departments.txt");

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
        cout << "5. View campus graph\n";
        cout << "6. Display graph counts and degrees\n";
        cout << "7. Find reachable buildings (BFS)\n";
        cout << "8. Find shortest path (BFS)\n";
        cout << "9. Add department\n";
        cout << "10. Search department\n";
        cout << "11. Display department hierarchy\n";
        cout << "12. Preorder traversal\n";
        cout << "13. Postorder traversal\n";
        cout << "14. Level-order traversal\n";
        cout << "15. Department statistics\n";
        cout << "16. Facility category report\n";
        cout << "17. Graph connectivity report\n";
        cout << "0. Exit\n";

        cout << "\nChoice: ";

        string input;
        if (!getline(cin, input))
        {
            choice = 0;
            break;
        }

        choice = -1;
        istringstream parser(input);
        if (!(parser >> choice))
        {
            cout << "invalid choice\n";
            continue;
        }

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
                cout << "enter facility name: ";
                getline(cin, name);

                if (trim(name).empty())
                {
                    cout << "error: facility name cannot be empty\n";
                    break;
                }

                facilityManager.searchByName(name);
                break;
            }

            case 4:
            {
                string category;
                cout << "enter category: ";
                getline(cin, category);

                if (trim(category).empty())
                {
                    cout << "error: category cannot be empty\n";
                    break;
                }

                facilityManager.searchByCategory(category);
                break;
            }

            case 5:
                campusGraph.displayGraph();
                break;

            case 6:
                campusGraph.displayCountsAndDegrees();
                break;

            case 7:
                campusGraph.reachableMenu();
                break;

            case 8:
                campusGraph.shortestPathMenu();
                break;

            case 9: {
                string name, parent;

                cout << "enter department name: ";
                getline(cin, name);
                name = trim(name);

                if (name.empty())
                {
                    cout << "error: department name cannot be empty\n";
                    break;
                }

                if (searchDepartment(deptTree->root, name))
                {
                    cout << "error: department already exists: " << name << "\n";
                    break;
                }

                if (!deptTree->root)
                {
                    // empty tree: first entry becomes the root
                    parent = "";
                }
                else
                {
                    cout << "enter parent name: ";
                    getline(cin, parent);
                    parent = trim(parent);

                    if (parent.empty())
                    {
                        cout << "error: parent name cannot be empty\n";
                        break;
                    }

                    if (!searchDepartment(deptTree->root, parent))
                    {
                        cout << "error: parent not found: " << parent << "\n";
                        break;
                    }
                }

                addDepartment(deptTree, name, parent);
                cout << "department added successfully\n";
                break;
            }

            case 10: {
                string name;
                cout << "enter name: ";
                getline(cin, name);
                name = trim(name);

                if (name.empty())
                {
                    cout << "error: department name cannot be empty\n";
                    break;
                }

                DepartmentNode* found = searchDepartment(deptTree->root, name);
                if (found)
                {
                    int depth = 0;
                    for (DepartmentNode* up = found->parent; up; up = up->parent)
                        ++depth;

                    cout << "name: " << found->name
                         << "\nparent: " << (found->parent ? found->parent->name : "none")
                         << "\nchildren: " << found->children.size()
                         << "\ndepth: " << depth
                         << endl;
                }
                else
                {
                    cout << "not found\n";
                }
                break;
            }

            case 11:
                if (!deptTree->root)
                    cout << "no departments loaded\n";
                else
                    displayHierarchy(deptTree->root, 0);
                break;

            case 12: {
                if (!deptTree->root)
                {
                    cout << "no departments loaded\n";
                    break;
                }

                vector<string> result;
                preorderTraversal(deptTree->root, result);
                for (size_t i = 0; i < result.size(); ++i)
                    cout << result[i] << (i + 1 < result.size() ? " " : "\n");
                break;
            }

            case 13: {
                if (!deptTree->root)
                {
                    cout << "no departments loaded\n";
                    break;
                }

                vector<string> result;
                postorderTraversal(deptTree->root, result);
                for (size_t i = 0; i < result.size(); ++i)
                    cout << result[i] << (i + 1 < result.size() ? " " : "\n");
                break;
            }

            case 14: {
                if (!deptTree->root)
                {
                    cout << "no departments loaded\n";
                    break;
                }

                vector<string> result;
                levelOrderTraversal(deptTree->root, result);
                for (size_t i = 0; i < result.size(); ++i)
                    cout << result[i] << (i + 1 < result.size() ? " " : "\n");
                break;
            }

            case 15:
                treeStatistics(deptTree);
                break;

            case 16:
                facilityManager.categoryReport();
                break;

            case 17:
                campusGraph.connectivityReport();
                break;

            case 0:
                break;

            default:
                cout << "invalid choice\n";
        }


    } while (choice != 0);

    delete deptTree;

    return 0;
}

