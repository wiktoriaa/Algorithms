#include <iostream>
#include <string>
#include <sstream>
using namespace std;

struct Node {
    int* values = nullptr;
    int size = 0;
    int childrenCount = 0;
    Node** children = nullptr;
    Node* parent = nullptr;
};

class Btree
{
public:
    Btree();
    ~Btree();
    void setOrder(int x);
    void insertValue(int value);
    void printTree(Node* node);
    Node* findValue(int value);
    Node* getRoot();
    int getOrder();
    void debug(Node* node, int depth);
    void remove(int value);
    void loadTree(int orderTree, string tree);
    string saveTree(Node* node);

    int recessLevel(int value);

private:
    bool isEmpty();
    bool isLeaf(Node* node);
    bool isValueOverflow(Node* node);
    void split(Node* node);
    void insertValIntoNode(Node* node, int value);
    Node* createNode(Node* parent);
    void switchNodes(Node*& a, Node*& b);
    void sortChildren(Node* node);
    void sortVals(Node* node);
    void insertChild(Node* parent, Node* child);
    void removeValueFromNode(Node* node, int index);
    void loadNode(Node* currNode, string tree);
    void removeValue(Node* node, int value);
    void clearTree(Node* node);
    void rebalance(Node* node);
    void removeChild(Node* parent, int index);
    int getChildIndex(Node* child);
    int getValueIndex(Node* node, int value);

    Node* findForInsertion(int value, Node* node);
    Node* root = nullptr;
    int order = 0;
    int valuesCount = 0;
};

Btree::Btree() {}

Btree::~Btree() {
    if (order > 0 && root != nullptr) {
        clearTree(root);
    }
}

void Btree::clearTree(Node* node) {
    if (node == nullptr)
        return;

    if (isLeaf(node)) {
        delete node;
        return;
    }

    for (int i = 0; i < node->childrenCount; i++) {
        clearTree(node->children[i]);
    }

    delete node;
}

int Btree::recessLevel(int value) {
    Node* tmp = root;
    bool tmpSwitch;
    int count = 0;

    while (tmp != nullptr) {
        tmpSwitch = false;
        count++;
        for (int i = 0; i < tmp->size; i++) {
            if (tmp->values[i] == value) return count;
            else if (value < tmp->values[i]) {
                tmp = tmp->children[i];
                tmpSwitch = true;
                break;
            }
        }
        if (!tmpSwitch && (tmp == nullptr || isLeaf(tmp))) break;
        if (!tmpSwitch)  {
            tmp = tmp->children[tmp->childrenCount - 1];
        }
    }

    return count;
}

Node* Btree::getRoot() { return root; }

int Btree::getOrder() { return order; }

bool Btree::isEmpty() {
    if (root == nullptr) return true;
    return false;
}

bool Btree::isLeaf(Node* node) {
    if (node->children[0] == nullptr) return true;
    return false;
}

bool Btree::isValueOverflow(Node* node) {
    if (node->size > valuesCount) return true;
    return false;
}

void Btree::setOrder(int x) {
    order = x;
    valuesCount = (2 * order) - 1;
}

void Btree::sortChildren(Node* node) {
    int childrenCount = node->childrenCount;

    for (int i = childrenCount - 1; i > 0; i--) {
        if (node->children[i]->values[0] < node->children[i - 1]->values[0]) {
            switchNodes(node->children[i], node->children[i - 1]);
        }
    }
}

void Btree::sortVals(Node* node) {
    int tmp;

    for (int i = node->size - 1; i > 0; i--) {
        if (node->values[i] < node->values[i - 1]) {
            tmp = node->values[i];
            node->values[i] = node->values[i - 1];
            node->values[i - 1] = tmp;
        }
        else break;
    }
}

Node* Btree::createNode(Node* parent) {
    if (order > 0) {
        Node* node = new Node;
        node->parent = parent;
        node->values = new int[valuesCount + 1];
        node->children = new Node * [valuesCount + 2];
        for (int i = 0; i < valuesCount + 2; i++) {
            node->children[i] = nullptr;
        }
        return node;
    }
    return nullptr;
}

void Btree::switchNodes(Node*& a, Node*& b) {
    Node* tmp = a;
    a = b;
    b = tmp;
}

void Btree::insertChild(Node* parent, Node* child) {
    for (int i = 0; i < valuesCount + 2; i++) {
        if (parent->children[i] == nullptr) {
            child->parent = parent;
            parent->children[i] = child;
            parent->childrenCount++;
            break;
        }
    }
    sortChildren(parent);
}

void Btree::removeValueFromNode(Node* node, int index) {
    for (int i = index; i < node->size - 1; i++) {
        node->values[i] = node->values[i + 1];
    }
    node->values[node->size - 1] = 0;
    node->size--;
}

void Btree::split(Node* node) {
    int midIndex = (node->size - 1) / 2;
    int midVal = node->values[midIndex];

    if (node == root) {
        Node* newNode = createNode(nullptr);
        Node* left = createNode(newNode);
        Node* right = createNode(newNode);

        for (int i = 0; i < midIndex; i++) {
            insertValIntoNode(left, node->values[i]);
        }
        for (int i = midIndex + 1; i < node->size; i++) {
            insertValIntoNode(right, node->values[i]);
        }

        if (!isLeaf(node)) {
            for (int i = 0; i <= midIndex; i++) {
                insertChild(left, node->children[i]);
                node->children[i] = nullptr;
                node->childrenCount--;
            }
            for (int i = midIndex + 1; i < valuesCount + 2; i++) {
                insertChild(right, node->children[i]);
                node->children[i] = nullptr;
                node->childrenCount--;
            }
        }

        insertChild(newNode, left);
        insertChild(newNode, right);
        insertValIntoNode(newNode, midVal);
        root = newNode;
    }
    else {
        Node* newChild = createNode(node->parent);

        for (int i = midIndex + 1; i < node->size; i++) {
            insertValIntoNode(newChild, node->values[i]);
        }

        for (int i = midIndex + 1; i < valuesCount + 2; i++) {
            if (node->children[i] != nullptr) {
                insertChild(newChild, node->children[i]);
                node->children[i] = nullptr;
                node->childrenCount--;
            }
        }

        int toDel = node->size;
        for (int i = midIndex; i < toDel; i++) {
            removeValueFromNode(node, i);
        }

        insertChild(node->parent, newChild);
        insertValIntoNode(node->parent, midVal);
    }
}

void Btree::insertValIntoNode(Node* node, int value) {
    node->values[node->size++] = value;
    sortVals(node);

    if (isValueOverflow(node)) {
        split(node);
    }
}

void Btree::insertValue(int value) {
    if (order == 0) return;

    if (isEmpty()) {
        root = createNode(nullptr);
        insertValIntoNode(root, value);
    }
    else {
        Node* leaf = findForInsertion(value, root);
        insertValIntoNode(leaf, value);
    }
}

void Btree::printTree(Node* node) {
    int i;
    for (i = 0; i < node->size; i++) {
        if (!isLeaf(node)) {
            printTree(node->children[i]);
        }
        printf("%d ", node->values[i]);
    }
    if (!isLeaf(node)) {
        printTree(node->children[i]);
    }
}

Node* Btree::findForInsertion(int value, Node* node) {
    if (isLeaf(node))
        return node;

    for (int i = 0; i < node->size; i++) {
        if (value < node->values[i]) {
            return findForInsertion(value, node->children[i]);
        }
    }

    return findForInsertion(value, node->children[node->childrenCount - 1]);
}

Node* Btree::findValue(int value) {
    Node* tmp = root;
    bool tmpSwitch;

    while (tmp != nullptr) {
        tmpSwitch = false;
        for (int i = 0; i < tmp->size; i++) {
            if (tmp->values[i] == value) return tmp;
            else if (value < tmp->values[i]) {
                tmp = tmp->children[i];
                tmpSwitch = true;
                break;
            }
        }
        if (!tmpSwitch && (tmp == nullptr || isLeaf(tmp))) break;
        if (!tmpSwitch) tmp = tmp->children[tmp->childrenCount - 1];
    }

    return nullptr;
}

void printIndentation(int depth) {
    for (int i = 0; i < depth; i++) {
        cout << "---";
    }
}

void Btree::debug(Node* node, int depth) {
    if (node == nullptr)
        return;

    if (isLeaf(node)) {
        printIndentation(depth);
        cout << '(';

        for (int i = 0; i < node->size; i++) {
            cout << node->values[i];

            if (i < node->size - 1)
                cout << " ";
        }

        cout << ')' << endl;
        return;
    }


    for (int i = 0; i < node->size; i++) {
        debug(node->children[i], depth + 1);
        printIndentation(depth);
        cout << '(' << node->values[i] << ')' << endl;
    }

    debug(node->children[node->childrenCount - 1], depth + 1);
}

void Btree::loadTree(int orderTree, string tree) {
    setOrder(orderTree);
    root = createNode(nullptr);
    loadNode(root, tree);
}

void Btree::loadNode(Node* currNode, string tree) {
    // remove external quotes
    tree = tree.substr(2, tree.size() - 4);

    stringstream ss;
    ss << tree;

    string data;
    int openCount = 0;
    int closeCount = 0;
    string childTree;


    while (ss >> data) {
        if (data == "(") {
            if (openCount == closeCount) {
                childTree = "( ";
            }
            else {
                childTree += "( ";
            }
            openCount++;
        }
        else if (data == ")") {
            closeCount++;
            childTree += ") ";

            if (openCount == closeCount) {
                Node* child = createNode(currNode);
                loadNode(child, childTree);
                insertChild(currNode, child);
            }
        }
        else if (openCount != closeCount) {
            childTree += data + " ";
        }
        else {
            insertValIntoNode(currNode, stoi(data));
        }
    }
}

string Btree::saveTree(Node* node) {
    string output = "( ";

    if (isLeaf(node)) {
        for (int i = 0; i < node->size; i++) {
            output += to_string(node->values[i]) + " ";
        }
    }
    else {
        int i;
        for (i = 0; i < node->size; i++) {
            output += saveTree(node->children[i]) + " ";
            output += to_string(node->values[i]) + " ";
        }
        output += saveTree(node->children[i]) + " ";
    }

    return output + ")";
}

void Btree::remove(int value) {
    Node* node = findValue(value);
    if (node == root && isLeaf(root) && root->size <= 1) {
        delete root;
        root = nullptr;
    }
    else if (node != nullptr) {
        removeValue(node, value);
    }
}

void Btree::removeValue(Node* node, int value) {
    int index = getValueIndex(node, value);
    if (index < 0) {
        printf("removeValue error\n");
        return;
    }

    Node* leftChild = node->children[index];
    Node* rightChild = node->children[index + 1];

    if (isLeaf(node)) {
        removeValueFromNode(node, index);

        if (node->size < order - 1) {
            rebalance(node);
        }
    }
    else {
        removeValueFromNode(node, index);

        if (leftChild->size > order - 1) {
            insertValIntoNode(node, leftChild->values[leftChild->size - 1]);
            removeValueFromNode(leftChild, leftChild->size - 1);
        }
        else if (rightChild->size > order - 1) {
            insertValIntoNode(node, rightChild->values[0]);
            removeValueFromNode(rightChild, 0);
        }
        else {
            // default right
            insertValIntoNode(node, rightChild->values[0]);
            removeValue(rightChild, rightChild->values[0]);
        }

    }
}

void Btree::rebalance(Node* node) {

    Node* parent = node->parent;
    int nodeIndexInParent = getChildIndex(node);

    Node* leftSibling;
    if (nodeIndexInParent > 0) {
        leftSibling = parent->children[nodeIndexInParent - 1];
    }
    else {
        leftSibling = nullptr;
    }

    Node* rightSibling;
    if (nodeIndexInParent < parent->childrenCount - 1) {
        rightSibling = parent->children[nodeIndexInParent + 1];
    }
    else {
        rightSibling = nullptr;
    }
    //int borrowedKey;

    // rotate right
    if (leftSibling != nullptr && leftSibling->size > order - 1) {
        if (!isLeaf(node)) {
            // z childa do node 1 dziecko index ostatni
            insertChild(node, leftSibling->children[leftSibling->childrenCount - 1]);
            leftSibling->children[leftSibling->childrenCount - 1] = nullptr;
            leftSibling->childrenCount--;
        }

        insertValIntoNode(node, parent->values[nodeIndexInParent - 1]);
        removeValueFromNode(parent, nodeIndexInParent - 1);

        insertValIntoNode(parent, leftSibling->values[leftSibling->size - 1]);
        removeValueFromNode(leftSibling, leftSibling->size - 1);
    }
    // rotate left
    else if (rightSibling != nullptr && rightSibling->size > order - 1) {
        if (!isLeaf(node)) {
            // z childa do node 1 dziecko index ostatni
            insertChild(node, leftSibling->children[0]);
            for (int i = 0; i < rightSibling->childrenCount - 1; i++) {
                rightSibling->children[i] = rightSibling->children[i + 1];
            }
            rightSibling->children[rightSibling->childrenCount - 1] = nullptr;
            leftSibling->childrenCount--;
        }

        insertValIntoNode(node, parent->values[nodeIndexInParent]);
        removeValueFromNode(parent, nodeIndexInParent);

        insertValIntoNode(parent, rightSibling->values[0]);
        removeValueFromNode(rightSibling, 0);
    }
    // merge
    else {
        // merge to right (exception only for the first child)
        if (nodeIndexInParent == 0) {
            int separatorIndex = 0;

            // move values from left to right
            for (int i = 0; i < node->size; i++) {
                insertValIntoNode(rightSibling, node->values[i]);
            }

            // move children from left to right
            for (int i = 0; i < node->childrenCount; i++) {
                insertChild(rightSibling, node->children[i]);
            }

            // remove child from parent
            removeChild(parent, nodeIndexInParent);

            insertValIntoNode(rightSibling, parent->values[separatorIndex]);
            removeValueFromNode(parent, separatorIndex);

            if (parent == root && parent->size == 0) {
                delete root;
                root = rightSibling;
                return;
            }
        }
        // merge to left (default)
        else {
            int separatorIndex = nodeIndexInParent - 1;

            // move values from right to left
            for (int i = 0; i < node->size; i++) {
                insertValIntoNode(leftSibling, node->values[i]);
            }

            // move children from right to left
            for (int i = 0; i < node->childrenCount; i++) {
                insertChild(leftSibling, node->children[i]);
            }

            // remove child from parent
            removeChild(parent, nodeIndexInParent);

            int val = parent->values[separatorIndex];

            removeValueFromNode(parent, separatorIndex);
            insertValIntoNode(leftSibling, val);

            if (parent == root && parent->size == 0) {
                delete root;
                root = leftSibling;
                return;
            }
        }

        // if rebalance is necessary
        if (parent->size < order - 1) {
            if (parent == root) {
                return;
            }

            if (!isLeaf(parent)) {
                rebalance(parent);
            }
            
        }
    }
}

void Btree::removeChild(Node* parent, int index) {
    delete parent->children[index];
    int i;
    for (i = index; i < parent->childrenCount; i++) {
        parent->children[i] = parent->children[i + 1];
    }
    parent->children[i] = nullptr;
    parent->childrenCount--;
}

int Btree::getChildIndex(Node* child) {
    Node* parent = child->parent;

    for (int i = 0; i < parent->childrenCount; i++) {
        if (parent->children[i] == child) {
            return i;
        }
    }
    return -1;
}

int Btree::getValueIndex(Node* node, int value) {
    for (int i = 0; i < node->size; i++) {
        if (node->values[i] == value) {
            return i;
        }
    }
    return -1;
}

int main()
{
    char command = '#';
    int val;
    string tree;
    Btree btree;

    int cacheCount, noCacheCount, count;
    int cacheSize;
    int* cache;
    bool found;
    string values;

    while (command != 'X') {
        cin >> command;

        switch (command)
        {
        case 'I':
            cin >> val;
            btree.setOrder(val);
            break;
        case 'A':
            cin >> val;
            btree.insertValue(val);
            break;
        case '?':
            cin >> val;
            if (btree.findValue(val) != nullptr) printf("%d +\n", val);
            else printf("%d -\n", val);
            break;
        case 'P':
            btree.printTree(btree.getRoot());
            printf("\n");
            break;
        case 'S':
            printf("%d\n", btree.getOrder());
            cout << btree.saveTree(btree.getRoot()) << "\n";
            break;
        case 'L':
            cin >> val;
            cin.ignore();
            getline(std::cin, tree);
            btree.loadTree(val, tree);
            break;
        case 'R':
            cin >> val;
            btree.remove(val); 
            break;
        case '#':
            break;
        case 'C':
            getline(cin, values);
            stringstream ss;
            ss << values;
            ss >> cacheSize;

            cache = new int[cacheSize];
            for (int i = 0; i < cacheSize; i++) {
                cache[i] = -1;
            }

            noCacheCount = 0;
            cacheCount = 0;

            while (ss >> val) {
                count = btree.recessLevel(val);
                noCacheCount += count;

                found = false;
                for (int i = 0; i < cacheSize; i++) {
                    if (cache[i] == val) {
                        found = true;
                        break;
                    }
                    else if (cache[i] == -1) break;
                }

                if (!found) {
                    cacheCount += count;

                    for (int i = 0; i < cacheSize; i++) {
                        if (i == cacheSize - 1) {
                            cache[i] = val;
                        }
                        else {
                            cache[i] = cache[i + 1];
                        }
                    }
                }
            }
            printf("NO CACHE: %d CACHE: %d", noCacheCount, cacheCount);
            delete[] cache;
            break;
        }
        //btree.debug(btree.getRoot(), 1);
    }
}

