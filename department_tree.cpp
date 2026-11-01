#include "department_tree.h"
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
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

// recursive heap cleanup
void destroyTree(DepartmentNode* node) {
    if (!node) return;
    for (auto child : node->children) {
        destroyTree(child);
    }
    delete node;
}
