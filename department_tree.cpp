#include "department_tree.h"
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <fstream>
#include <queue>
using namespace std;

// trim surrounding whitespace
string trim(const string& s) {
    size_t first = s.find_first_not_of(" \t\r\n");
    if (first == string::npos) return "";
    size_t last = s.find_last_not_of(" \t\r\n");
    return s.substr(first, (last - first + 1));
}

// case insensitive comparison
bool iequals(const string& a, const string& b) {
    if (a.length() != b.length()) return false;
    for (size_t i = 0; i < a.length(); ++i) {
        if (tolower(static_cast<unsigned char>(a[i])) != tolower(static_cast<unsigned char>(b[i]))) return false;
    }
    return true;
}

// alocate empty tree
DepartmentTree* createTree() {
    return new DepartmentTree{nullptr};
}

// destructor frees every node through destroyTree
DepartmentTree::~DepartmentTree() {
    destroyTree(root);
}

// recursive case insensitive search
DepartmentNode* searchDepartment(DepartmentNode* node, const string& name) {
    if (!node) return nullptr;
    if (iequals(node->name, name)) return node;
    for (auto child : node->children) {
        DepartmentNode* found = searchDepartment(child, name);
        if (found) return found;
    }
    return nullptr;
}

// validate and inserrt node
void addDepartment(DepartmentTree* tree, const string& name, const string& parentName) {
    string trimmedName = trim(name);
    string trimmedParent = trim(parentName);

    if (trimmedName.empty()) return;
    if (searchDepartment(tree->root, trimmedName)) return;

    if (!tree->root) {
        tree->root = new DepartmentNode{trimmedName, nullptr, {}};
        return;
    }

    DepartmentNode* parent = searchDepartment(tree->root, trimmedParent);
    if (parent) {
        parent->children.push_back(new DepartmentNode{trimmedName, parent, {}});
    }
}

// recursive indented print
void displayHierarchy(DepartmentNode* node, int depth) {
    if (!node) return;
    for (int i = 0; i < depth; ++i) cout << "    ";
    cout << node->name << "\n";
    for (auto child : node->children) {
        displayHierarchy(child, depth + 1);
    }
}

// recursive preorder list
void preorderTraversal(DepartmentNode* node, vector<string>& result) {
    if (!node) return;
    result.push_back(node->name);
    for (auto child : node->children) {
        preorderTraversal(child, result);
    }
}

// children first, then the node
void postorderTraversal(DepartmentNode* node, vector<string>& result) {
    if (!node) return;
    for (auto child : node->children) {
        postorderTraversal(child, result);
    }
    result.push_back(node->name);
}

// queue based breadth first visit order
void levelOrderTraversal(DepartmentNode* node, vector<string>& result) {
    if (!node) return;
    queue<DepartmentNode*> pending;
    pending.push(node);
    while (!pending.empty()) {
        DepartmentNode* current = pending.front();
        pending.pop();
        result.push_back(current->name);
        for (auto child : current->children) {
            pending.push(child);
        }
    }
}

// height of a subtree counted in edges
static int subtreeHeight(DepartmentNode* node) {
    if (!node) return -1;
    int best = -1;
    for (auto child : node->children) {
        int height = subtreeHeight(child);
        if (height > best) best = height;
    }
    return best + 1;
}

// recursive totals and leaf count
static void countNodes(DepartmentNode* node, int& total, int& leaves) {
    if (!node) return;
    ++total;
    if (node->children.empty()) ++leaves;
    for (auto child : node->children) {
        countNodes(child, total, leaves);
    }
}

// totals leaves and tree height
void treeStatistics(DepartmentTree* tree) {
    if (!tree->root) {
        cout << "no departments loaded\n";
        cout << "total departments: 0\n";
        cout << "leaf departments: 0\n";
        cout << "tree height: n/a\n";
        return;
    }

    int total = 0;
    int leaves = 0;
    countNodes(tree->root, total, leaves);

    cout << "total departments: " << total << "\n";
    cout << "leaf departments: " << leaves << "\n";
    cout << "tree height: " << subtreeHeight(tree->root) << "\n";
}

// recursive heap cleanup
void destroyTree(DepartmentNode* node) {
    if (!node) return;
    for (auto child : node->children) {
        destroyTree(child);
    }
    delete node;
}
// load departments from txt
void loadDepartments(DepartmentTree* tree, const string& filename) {
    ifstream file(filename);
    if (!file) {
        cout << "could not open " << filename << "\n";
        return;
    }

    string line;
    while (getline(file, line)) {
        line = trim(line);
        if (line.empty() || line[0] == '#') continue;

        size_t bar = line.find('|');
        if (bar == string::npos) {
            addDepartment(tree, line, "");
            continue;
        }

        string name = trim(line.substr(0, bar));
        string parent = trim(line.substr(bar + 1));


        if (iequals(parent, "NONE")) parent = "";
        addDepartment(tree, name, parent);
    }
}
