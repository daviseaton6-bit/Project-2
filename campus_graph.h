#ifndef CAMPUSGRAPH_H
#define CAMPUSGRAPH_H

#include <string>
#include <vector>
using namespace std;

class CampusGraph
{
public:
	CampusGraph();
	
	void loadBuildings(string folder);
	void loadConnections(string folder);

	int findBuilding(string name) const;
	string buildingName(int number) const;

	void displayGraph();
	void displayCountsAndDegrees();
	void reachableMenu();
	

private:
	vector<string> names;
	vector < vector<int> > neighbors;
	int connectionCount;

};

#endif
