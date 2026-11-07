#include "CampusGraph.h"
#include <iostream>
#include <fstream>
#include <queue>
using namespace std;

string graphTrim(string word)
{
    int start = 0;
    int end = word.size() - 1;

    while (start <= end && (word[start] == ' ' || word[start] == '\t' || word[start] == '\r'))
    {
        start = start + 1;
    }
    while (end >= start && (word[end] == ' ' || word[end] == '\t' || word[end] == '\r'))
    {
        end = end - 1;
    }
    return word.substr(start, end - start + 1);
}

string graphLower(string word)
{
    int size = word.size();
    for (int i = 0; i < size; i++)
    {
        if (word[i] >= 'A' && word[i] <= 'Z')
        {
            word[i] = word[i] + 32;
        }
    }
    return word;
}

vector<string> graphSplit(string line)
{
    vector<string> pieces;
    string current = "";

    int size = line.size();
    for (int i = 0; i < size; i++)
    {
        if (line[i] == '|') 
        {
            pieces.push_back(graphTrim(current));
            current = "";
        }
        else
        {
            current = current + line[i];
        }
    }
    pieces.push_back(graphTrim(current));
    return pieces;
}

// constructor and lookup

CampusGraph:: CampusGraph()
{
    connectionCount = 0;
}
 // find buildings by name
int CampusGraph:: findBuilding(string name) const
{
    string wanted = graphLower(graphTrim(name));

    int total = names.size();
    for (int i = 0; i < total; i++)
    {
        if (graphLower(names[i]) == wanted)
        {
            return i;
        }
    }
    return -1;

}

string CampusGraph::buildingName(int number) const
{ 
    return names[number];
}

// Reading buildings .txt

void CampusGraph::loadBuildings(string folder)
{
    string filename = "buildings.txt";
    int accepted = 0;
    int rejected = 0;

     string path = folder + "/" + filename;
    if (folder.size() >= 4 && folder.substr(folder.size() - 4) == ".txt") {
        path = folder;}

    ifstream file(path.c_str());
    
    if (!file)
    {
        cout << "Error: could not open " << filename << " (treating it as empty )" << endl;
        cout << "Loaded " << filename << ": 0 accepted, 0 rejected" << endl;
        return; 
    }

    string line;
    int lineNumber = 0;

    while (getline(file, line))
    {
        lineNumber = lineNumber + 1;

        string clean = graphTrim(line);

        if (clean == "")
        {
            continue;
        }
        if (clean[0] == '#')
        {
            continue;
        }

        vector<string> pieces = graphSplit(line);

        if (pieces.size() != 1)
        {
            cout << "Warning: " << filename << "line " <<
                lineNumber << ": wrong number of fields" << endl;
            rejected = rejected + 1;
        }
        else if (findBuilding(pieces[0]) != -1)
        {
            cout << "warning: " << filename << " line " << lineNumber
                << ": duplicate building " << pieces[0] << endl;
            rejected = rejected + 1;
        }
        else 
        {
            names.push_back(pieces[0]);

            vector<int> emptyList;
            neighbors.push_back(emptyList);

            accepted = accepted + 1;
        }
    }
    cout << "Loaded " << filename << ": " << accepted << "accepted, "
        << rejected << " rejected" << endl;
}

// Reading connections . txt

void CampusGraph::loadConnections(string folder)
{
    string filename = "connections.txt";
    int accepted = 0;
    int rejected = 0;

        string path = folder + "/" + filename;
    if (folder.size() >= 4 && folder.substr(folder.size() - 4) == ".txt") {
        path = folder;}


    ifstream file(path.c_str());

    if (!file)
    {
        cout << "Error: could not open " << filename << " (treating if empty) " << endl;
        cout << "Loaded " << filename << " 0 accepted, 0 rejected " << endl;
        return;
    }

    string line;
    int lineNumber = 0;

    while (getline(file, line))
    {
        lineNumber = lineNumber + 1;

        string clean = graphTrim(line);
        if (clean == "")
        {
            continue;
        }
        if (clean[0] == '#')
        {
            continue;
        }

        vector<string> pieces = graphSplit(line);

        if (pieces.size() != 2)
        {
            cout << "Warning: " << filename << " line " << lineNumber
                << ": wrong number of fields" << endl;
            rejected = rejected + 1;
            continue;
        }

        if (pieces[0] == "" || pieces[1] == "")
        {
            cout << "Warning: " << filename << " line " << lineNumber
                << ": empty field" << endl;
            rejected = rejected + 1;
            continue;
        }

        int a = findBuilding(pieces[0]);
        int b = findBuilding(pieces[1]);
        
        if (a == -1)
        {
            cout << "Warning: " << filename << " line " << lineNumber
                << ": unknown building " << pieces[0] << endl;
            rejected = rejected + 1;
            continue;
        }
        if (b == -1) {
            cout << "Warning: " << filename << " line " << lineNumber
                << ": unknown building " << pieces[1] << endl;
            rejected = rejected + 1;
            continue;
        }
        if (a == b) {
            cout << "Warning: " << filename << " line " << lineNumber
                << ": building connects to itself" << endl;
            rejected = rejected + 1;
            continue;
        }
        bool alreadyThere = false;
        int count = neighbors[a].size();
        for (int i = 0; i < count; i++) {
            if (neighbors[a][i] == b) {
                alreadyThere = true;
            }
        }
        if (alreadyThere == true) {
            cout << "Warning: " << filename << " line " << lineNumber
                << ": duplicate connection" << endl;
            rejected = rejected + 1;
            continue;
        }
        neighbors[a].push_back(b);
        neighbors[b].push_back(a);
        connectionCount = connectionCount + 1;
        accepted = accepted + 1;
    }

    cout << "Loaded " << filename << ": " << accepted << " accepted, "
        << rejected << " rejected" << endl;
}

// show the graph

void CampusGraph::displayGraph() {
    int total = names.size();

    if (total == 0) {
        cout << "There are no buildings loaded." << endl;
        return;
    }

    cout << "Campus graph:" << endl;

    for (int i = 0; i < total; i++) {
        cout << "  " << names[i] << ": ";

        int howMany = neighbors[i].size();

        if (howMany == 0) {
            cout << "(no connections)";
        }
        else {
            for (int j = 0; j < howMany; j++) {
                if (j > 0) {
                    cout << ", ";
                }
                int friendNumber = neighbors[i][j];
                cout << names[friendNumber];
            }
        }
        cout << endl;
    }
}

//counts and degrees

void CampusGraph::displayCountsAndDegrees() {
    int total = names.size();

    cout << "Total buildings: " << total << endl;
    cout << "Total connections: " << connectionCount << endl;

    if (total == 0) {
        cout << "There are no buildings loaded." << endl;
        return;
    }

    cout << "Degree of each building:" << endl;
    for (int i = 0; i < total; i++) {
        // degree = number of neighbors
        cout << "  " << names[i] << ": " << neighbors[i].size() << endl;
    }
}

// BFS

void CampusGraph::reachableMenu() {
    cout << "Enter starting building: ";
    string typed;
    getline(cin, typed);
    typed = graphTrim(typed);

    if (typed == "") {
        cout << "Error: the building name cannot be blank." << endl;
        return;
    }

    int start = findBuilding(typed);
    if (start == -1) {
        cout << "Error: unknown building " << typed << endl;
        return;
    }

    int total = names.size();

    vector<int> visited(total, 0);

    vector<int> distance(total, 0);

    queue<int> line;

    visited[start] = 1;     
    line.push(start);

    int reachableCount = 0;
    cout << "Buildings reachable from " << names[start] << ":" << endl;

    while (line.empty() == false) {
        int current = line.front();   
        line.pop();

        cout << "  " << names[current] << " (distance " << distance[current] << ")" << endl;
        reachableCount = reachableCount + 1;

        int howMany = neighbors[current].size();
        for (int i = 0; i < howMany; i++) {
            int next = neighbors[current][i];

            if (visited[next] == 0) {
                visited[next] = 1;
                distance[next] = distance[current] + 1;
                line.push(next);
            }
        }
    }

    cout << "Reachable: " << reachableCount << " out of " 
        << total << " buildings" << endl;
}
