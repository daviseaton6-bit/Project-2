#ifndef DEPARTMENT_TREE_H
#define DEPARTMENT_TREE_H

#include <string>
#include <vector>
#include <fstream>

using namespace std;

struct DepartmentNode {
    string name;
    DepartmentNode* parent;
    vector<DepartmentNode*> children;
};

struct DepartmentTree {
    DepartmentNode* root;
    // frees all nodes when the tree is deleted
    ~DepartmentTree();
};

DepartmentTree* createTree();
void addDepartment(DepartmentTree* tree, const string& name, const string& parentName);
DepartmentNode* searchDepartment(DepartmentNode* node, const string& name);
void displayHierarchy(DepartmentNode* node, int depth);
void preorderTraversal(DepartmentNode* node, vector<string>& result);
void postorderTraversal(DepartmentNode* node, vector<string>& result);
void levelOrderTraversal(DepartmentNode* node, vector<string>& result);
void treeStatistics(DepartmentTree* tree);
string trim(const string& s);
void destroyTree(DepartmentNode* node);
void loadDepartments(DepartmentTree* tree, const string& filename);

#endif
