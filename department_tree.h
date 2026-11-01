#ifndef DEPARTMENT_TREE_H
#define DEPARTMENT_TREE_H

#include <string>
#include <vector>

using namespace std;

struct DepartmentNode {
    string name;
    DepartmentNode* parent;
    vector<DepartmentNode*> children;
};

struct DepartmentTree {
    DepartmentNode* root;
};

DepartmentTree* createTree();
void addDepartment(DepartmentTree* tree, const string& name, const string& parentName);
DepartmentNode* searchDepartment(DepartmentNode* node, const string& name);
void displayHierarchy(DepartmentNode* node, int depth);
void preorderTraversal(DepartmentNode* node, vector<string>& result);
void destroyTree(DepartmentNode* node);

#endif
