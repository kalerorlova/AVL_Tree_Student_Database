#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
using namespace std;
/* Note: 
	1. You will have to comment main() when unit testing your code because catch uses its own main().
	2. You will submit this main.cpp file and any header files you have on Gradescope. 
*/

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
    MyAVLTree();                            //constructor & destructor
    void destructorHelper(BiNode* &root);
    ~MyAVLTree();

    BiNode* rotateCaseLL(BiNode* &node);     //rotations
    BiNode* rotateCaseLR(BiNode* &node);
    BiNode* rotateCaseRL(BiNode* &node);
    BiNode* rotateCaseRR(BiNode* &node);

    int getHeight(BiNode* &node);                  //balance and insertion
    BiNode* balance(BiNode* &root, int height);
    BiNode* insertHelper(BiNode* &root, string name, int ID);
    void insert(string name, int ID);

    BiNode* findSuccessor(BiNode* &root);            //removal
    BiNode* removeHelper(BiNode* &root, int ID);
    void remove(int ID);

    bool searchHelper(BiNode* &root, int ID);    //search by ID
    void search(int ID);

    bool searchHelper(BiNode* &root, string name, bool &foundName); //search by name
    void search(string name);

    void printHelper(vector<string> &names);    //printing in format

    void printInorderHelper(BiNode* &root, vector<string> &names);  //inorder traversal
    void printInorder();
    
    void printPreorderHelper(BiNode* &root, vector<string> &names);  //preorder traversal
    void printPreorder();

    void printPostorderHelper(BiNode* &root, vector<string> &names); //postorder traversal
    void printPostorder();

    void printLevelCount();       //level count
    
    void removeInorderHelper(BiNode* &root, int N, int &count);       //remove in order
    void removeInorder(int N);

};

//==================IMPLEMENTATION===========================

//CONSTRUCTOR & DESTRUCTOR

MyAVLTree::MyAVLTree() {
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

//ROTATIONS

MyAVLTree::BiNode* MyAVLTree::rotateCaseLL(BiNode* &node) {  //right rotation
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
    node->right = rotateCaseLL(node->right);
    return rotateCaseRR(node);
}

MyAVLTree::BiNode* MyAVLTree::rotateCaseRR(BiNode* &node) { //left rotation
    BiNode* parent = node->right;
    node->right = parent->left;
    parent->left = node;
    node->height = 1 + max(getHeight(node->left), getHeight(node->right));
    parent->height = 1 + max(getHeight(parent->left), getHeight(parent->right));
    return parent;
}

//BALANCE AND INSERTION

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
    root->height = 1 + max(getHeight(root->left), getHeight(root->right));
    int bf = getHeight(root->left) - getHeight(root->right);
    //cout << "Balance of the root " << root->ID << " is " << bf << endl;
    root = balance(root, bf);
    return root;
}

void MyAVLTree::insert(string name, int ID) {
    insertHelper(root, name, ID);
}

//REMOVAL

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

//SEARCH BY ID

bool MyAVLTree::searchHelper(BiNode* &root, int ID) {
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

//SEARCH BY NAME

bool MyAVLTree::searchHelper(BiNode* &root, string name, bool &foundName) {
    if (root == nullptr) {
        return false;
    }
    else {
        if (name == root->name) {
            cout << setw(8) << setfill('0') << root->ID << endl;
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

//PRINTING AND TRAVERSALS

void MyAVLTree::printHelper(vector<string> &names) {
    for (int i = 0; i < names.size()-1; i++) {
        if (names.size() == 0) {
            continue;
        }
        cout << names[i] << ", ";
    }
    if (names.size() != 0) {
        cout << names[names.size()-1] << endl;
    }
}

void MyAVLTree::printInorderHelper(BiNode* &root, vector<string> &names) {
    if (root == nullptr) {
        return;
    }
    else {
        printInorderHelper(root->left, names);
        names.push_back(root->name);
        printInorderHelper(root->right, names);
    }
    return;
}

void MyAVLTree::printInorder() {
    vector<string> names;
    printInorderHelper(root, names);
    printHelper(names);
}

void MyAVLTree::printPreorderHelper(BiNode* &root, vector<string> &names) {
    if (root == nullptr) {
        return;
    }
    else {
        names.push_back(root->name);
        printPreorderHelper(root->left, names);
        printPreorderHelper(root->right, names);
    }
    return;
}

void MyAVLTree::printPreorder() {
    vector<string> names;
    printPreorderHelper(root, names);
    printHelper(names);
}

void MyAVLTree::printPostorderHelper(BiNode* &root, vector<string> &names) {
    if (root == nullptr) {
        return;
    }
    else {
        printPostorderHelper(root->left, names);
        printPostorderHelper(root->right, names);
        names.push_back(root->name);
    }
    return;
}

void MyAVLTree::printPostorder() {
    vector<string> names;
    printPostorderHelper(root, names);
    printHelper(names);
}

//LEVEL COUNT

void MyAVLTree::printLevelCount() {
    //cout << getHeight(root) << " is the height of the root which is " << root->name << endl;
    cout << getHeight(root) << endl;
}

//REMOVAL IN ORDER

void MyAVLTree::removeInorderHelper(BiNode* &root, int N, int &count) {
    if (root == nullptr) {
        return;
    }
    else {
        removeInorderHelper(root->left, N, count);
        count++;
        if (count == N) {
            //cout << "Removing name " << root->name << endl;
            removeHelper(root, root->ID);
        }
        else {
            removeInorderHelper(root->right, N, count);
        }
    }
    return;
}

void MyAVLTree::removeInorder(int N) {
    int count = -1;
    removeInorderHelper(root, N, count);
    if (count < N) {
        cout << "unsuccessful" << endl;
    }
    return;
}

//================================ MAIN-RELATED ======================================

//VALIDATION

bool validateName(string name) {
	bool alpha = true;
	for (int i = 0; i < name.length(); i++) {
		if (!isalpha(name[i]) && !isspace(name[i])) {
			alpha = false;
			//cout << "unsuccessful" << endl;
			break;
		}
	}
	return alpha;
}

bool validateLength(string num) {
    if (num.length() != 8) {
		return false;
	}
    else return true;
}

bool validateNum(string num) {
    bool digit = true;
	for (int i = 0; i < num.length(); i++) {
		if (!isdigit(num[i])) {
			digit = false;
			//cout << "unsuccessful" << endl;
			break;
		}
	}
	return digit;
}

bool validateID(string id) {
	return validateLength(id) && validateNum(id);
}

//MAIN

int main(){
	int numCommands;					//get and parse input
	vector<vector<string>> commands;
	cin >> numCommands;                 
	string line;
	getline(cin, line);

	while(numCommands--) {
		vector<string> command;
		getline(cin, line);
		istringstream forParsing(line);
		string keyword, name, id;
		forParsing >> keyword >> quoted(name) >> id;
		command.push_back(keyword);
		command.push_back(name);
		command.push_back(id);
		commands.push_back(command);
	}

	MyAVLTree tree;		//create an instance of the AVLtree
	
	for (int i = 0; i < commands.size(); i++) {		//loop over all the commands
		if (commands[i].size() > 3) {				//if invalid number of words in the command
			cout << "unsuccessful" << endl;
			continue;
		}
		else {
			if (commands[i][0] == "insert") {				//insert if valid command
				if (!validateName(commands[i][1])) {
					cout << "unsuccessful" << endl;
					continue;
				}
				if (!validateID(commands[i][2])) {
					cout << "unsuccessful" << endl;
					continue;
				}
				tree.insert(commands[i][1], stoi(commands[i][2]));
			}
			else if (commands[i][0] == "remove") {			//remove if valid command
				if (validateID(commands[i][1])) {
					tree.remove(stoi(commands[i][1]));
				}
			}
			else if (commands[i][0] == "search") {			//search if valid command
				if (validateID(commands[i][1])) {			//search ID
					tree.search(stoi(commands[i][1]));
				}
				else if (validateName(commands[i][1])) {										//search name
					tree.search(commands[i][1]);
				}
				else {
					cout << "unsuccessful" << endl;
				}
			}
			else if (commands[i][0] == "printInorder") {			//printInorder
				tree.printInorder();
			}
			else if (commands[i][0] == "printPreorder") {			//printPreorder
				tree.printPreorder();
			}
			else if (commands[i][0] == "printPostorder") {			//printPostorder
				tree.printPostorder();
			}
			else if (commands[i][0] == "printLevelCount") {			//printLevelCount
				tree.printLevelCount();
			}
			else if (commands[i][0] == "removeInorder") {			//removeInorder
				if (validateNum(commands[i][1])) {
                    tree.removeInorder(stoi(commands[i][1]));
                }
			}
            else {
                cout << "unsuccessful" << endl;
            }
		}
	}
	return 0;
}

