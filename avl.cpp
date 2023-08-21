#pragma once
#include <iostream>
#include <string>
#include <vector>
using namespace std;

class MyAVLTree {
    struct BiNode {
        int ID;
        string name;
        BiNode* left;
        BiNode* right;
        int height;

        BiNode() : left(nullptr), right(nullptr), height(0) {};
        BiNode(int id, string name) : ID(id), name(name), left(nullptr), right(nullptr), height(1) {};
    };
    BiNode* root;

    public:
    MyAVLTree();
    void destructorHelper(BiNode* &root);
    ~MyAVLTree();

    BiNode* rotateCaseLL(BiNode* &node);     //rotations - done?
    BiNode* rotateCaseLR(BiNode* &node);
    BiNode* rotateCaseRL(BiNode* &node);
    BiNode* rotateCaseRR(BiNode* &node);

    int getHeight(BiNode* &node);                  //balance and insert - done?
    BiNode* balance(BiNode* &root, int height);
    BiNode* insertHelper(BiNode* &root, string name, int ID);
    void insert(string name, int ID);

    BiNode* findSuccessor(BiNode* &root);            //done?
    BiNode* removeHelper(BiNode* &root, int ID);
    void remove(int ID);

    bool searchHelper(BiNode* &root, int ID);    //done?
    void search(int ID);

    bool searchHelper(BiNode* &root, string name, bool &foundName); //done? maybe mistakes
    void search(string name);

    void printInorderHelper(BiNode* &root, vector<string> &names);  //done?
    void printInorder();
    
    void printPreorderHelper(BiNode* &root);  //done?
    void printPreorder();

    void printPostorderHelper(BiNode* &root); //done?
    void printPostorder();

    void printLevelCount();       //done? maybe mistakes
    
    void removeInorderHelper(BiNode* &root, int N);       //done?
    void removeInorder(int N);

};

//==================IMPLEMENTATION===========================

MyAVLTree::MyAVLTree() {
    //root = new BiNode;
    root = nullptr;
}

void MyAVLTree::destructorHelper(BiNode* &root) {
    if (root == nullptr) {
        return;
    }
    else {
        if (root->left == nullptr && root->right == nullptr) {
            delete root;
            root = nullptr;
        }
        else {
            destructorHelper(root->left);
            destructorHelper(root->right);
        }
    }
}

MyAVLTree::~MyAVLTree() {
    destructorHelper(root);
}

MyAVLTree::BiNode* MyAVLTree::rotateCaseLL(BiNode* &node) {  //right rotation
    //cout << "I enter this rotation on " << root->name << endl;
    BiNode* parent = node->left;
    node->left = parent->right;
    parent->right = node;
    node->height = 1 + max(getHeight(node->left), getHeight(node->right));
    parent->height = 1 + max(getHeight(parent->left), getHeight(parent->right));
    return parent;
}

MyAVLTree::BiNode* MyAVLTree::rotateCaseLR(BiNode* &node) {
    node->left = rotateCaseRR(node->left);
    return rotateCaseLL(node);
}

MyAVLTree::BiNode* MyAVLTree::rotateCaseRL(BiNode* &node) {
    node->right = rotateCaseLL(node->left);
    return rotateCaseRR(node);
}

MyAVLTree::BiNode* MyAVLTree::rotateCaseRR(BiNode* &node) { //left rotation
    //cout << "I enter this rotation on " << root->name << endl;
    BiNode* parent = node->right;
    node->right = parent->left;
    parent->left = node;
    node->height = 1 + max(getHeight(node->left), getHeight(node->right));
    parent->height = 1 + max(getHeight(parent->left), getHeight(parent->right));
    return parent;
}

int MyAVLTree::getHeight(BiNode* &node) {
    if (node == nullptr) {
        return 0;
    }
    else {
        return node->height;
    }
}

MyAVLTree::BiNode* MyAVLTree::balance(BiNode* &root, int bf) {
    //cout << "Balance is " << bf << endl;
    if (bf == -2) {
        if (getHeight(root->right->left) - getHeight(root->right->right) == -1) {
            return rotateCaseRR(root);
        }
        else {
            return rotateCaseRL(root);
        }
    }
    else if (bf == 2) {
        if (getHeight(root->left->left) - getHeight(root->left->right) == -1) {
            return rotateCaseLR(root);
        }
        else {
            return rotateCaseLL(root);
        }
    }
    return root;
}

MyAVLTree::BiNode* MyAVLTree::insertHelper(BiNode* &root, string name, int ID) {
    //cout << "ROOT ID " << root->ID << endl;
    if (root == nullptr) {
        root = new BiNode(ID, name);
        cout << "successful" << endl;
        //root->height = 1 + max(getHeight(root->left), getHeight(root->right));
        //cout << "in nullptr section, root heights are: Parent " << root->height << ", Left  " << getHeight(root->left) <<  ", Right  " << getHeight(root->right) << endl;
        //return root;
    }
    else if (ID < root->ID) {
        root->left = insertHelper(root->left, name, ID);
    }
    else if (ID == root->ID) {
        cout << "unsuccessful" << endl;
    }
    else {
        root->right = insertHelper(root->right, name, ID);
    }
    ////root->height = root->left->height - root->right->height;
    root->height = 1 + max(getHeight(root->left), getHeight(root->right));
    //cout << "root heights are: Parent " << root->height << ", Left  " << getHeight(root->left) <<  ", Right  " << getHeight(root->right) << endl;
    int bf = getHeight(root->left) - getHeight(root->right);
    root = balance(root, bf);
    return root;
}

void MyAVLTree::insert(string name, int ID) {
    insertHelper(root, name, ID);
}

MyAVLTree::BiNode* MyAVLTree::findSuccessor(BiNode* &root) {
    BiNode* successor = root;
    while (successor->left != nullptr) {
        successor = successor->left;
    }
    return successor;
}

MyAVLTree::BiNode* MyAVLTree::removeHelper(BiNode* &root, int ID) {
    if (root == nullptr) {              // no ID is found or null tree
        cout << "unsuccessful" << endl;
    }
    else if (ID < root->ID) {           //remove on the left
        root->left = removeHelper(root->left, ID);
    }
    else if (ID > root->ID) {           //remove on the right
        root->right = removeHelper(root->right, ID);
    }
    else {          //found ID
        if (root->left == nullptr && root->right == nullptr) {      // no children
            delete root;
            root = nullptr;
            cout << "successful" << endl;
        }
        else if (root->left != nullptr && root->right != nullptr) {   // two children
            BiNode* successor = findSuccessor(root->right);
            root->name = successor->name;
            root->ID = successor->ID;
            root->right = removeHelper(root->right, successor->ID);
        }
        else {                          //one child
            BiNode* temp;
            if (root->left != nullptr) {
                temp = root->left;
            }
            else {
                temp = root->right;
            }
            delete root;
            root = temp;
            cout << "successful" << endl;
        }
    }
    if (root != nullptr) {
        root->height = 1 + max(getHeight(root->left), getHeight(root->right));
    }
    return root;
}

void MyAVLTree::remove(int ID) {
    removeHelper(root, ID);
}

bool MyAVLTree::searchHelper(BiNode* &root, int ID) {
    //cout << "Will be printed once in searchHelper is the root is nullptr" << endl;
    if (root == nullptr) {
        cout << "unsuccessful" << endl;
        return false;
    }
    else if (ID == root->ID) {
        cout << root->name << endl;
        return true;
    }
    else if (ID < root->ID) {
        return searchHelper(root->left, ID);
    }
    else {
        return searchHelper(root->right, ID);
    }
}

void MyAVLTree::search(int ID) {
    searchHelper(root, ID);
}

bool MyAVLTree::searchHelper(BiNode* &root, string name, bool &foundName) {
    if (root == nullptr) {
        return false;
    }
    else {
        if (name == root->name) {
            cout << root->ID << endl;
            foundName = true;
        }
        searchHelper(root->left, name, foundName);
        searchHelper(root->right, name, foundName);
    }
    return foundName;
} 

void MyAVLTree::search(string name) {
    bool foundName = false;
    searchHelper(root, name, foundName);
    if (!foundName) {
        cout << "unsuccessful" << endl;
    }
}

void MyAVLTree::printInorderHelper(BiNode* &root, vector<string> &names) {
    if (root == nullptr) {
        return;
    }
    else {
        printInorderHelper(root->left, names);
        //cout << root->name << ", ";
        names.push_back(root->name);
        printInorderHelper(root->right, names);
    }
    return;
}

void MyAVLTree::printInorder() {
    vector<string> names;
    printInorderHelper(root, names);
    for (int i = 0; i < names.size()-1; i++) {
        cout << names[i] << ", ";
    }
    cout << names[names.size()-1] << endl;
}

void MyAVLTree::printPreorderHelper(BiNode* &root) {
    if (root == nullptr) {
        return;
    }
    else {
        cout << root->name << ", ";
        printPreorderHelper(root->left);
        printPreorderHelper(root->right);
    }
    return;
}

void MyAVLTree::printPreorder() {
    printPreorderHelper(root);
}

void MyAVLTree::printPostorderHelper(BiNode* &root) {
    if (root == nullptr) {
        return;
    }
    else {
        printPostorderHelper(root->left);
        printPostorderHelper(root->right);
        cout << root->name << ", ";
    }
    return;
}

void MyAVLTree::printPostorder() {
    printPostorderHelper(root);
}

void MyAVLTree::printLevelCount() {
    //cout << getHeight(root) << " is the height of the root which is " << root->name << endl;
    cout << getHeight(root) << endl;
}

void MyAVLTree::removeInorderHelper(BiNode* &root, int N) {
    if (root == nullptr) {
        //cout << "unsuccessful" << endl;
        return;
    }
    else {
        removeInorderHelper(root->left, N);
        N--;
        if (N == 0) {
            //cout << "Removing name " << root->name << endl;
            removeHelper(root, root->ID);
            return;
        }
        removeInorderHelper(root->right, N);/*
        N--;
        if (N == 0) {
            removeHelper(root, root->ID);
        }*/
    }
    return;
}

void MyAVLTree::removeInorder(int N) {
    removeInorderHelper(root, N);
}