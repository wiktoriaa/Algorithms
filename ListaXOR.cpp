#include <iostream>
#include <string>
using namespace std;

struct Node
{
    int value;
    Node* prevNext;
};

Node* XOR(Node* x, Node* y)
{
    return (Node*)(((uintptr_t)x) ^ ((uintptr_t)y));
}

class XorList
{
public:
    XorList();
    ~XorList();
    Node* addBeg(int value);
    Node* addEnd(int value);
    void insertBefore(Node *node, int val);
    Node* delBeg();
    Node* delEnd();
    void delNode(Node* node);
    void delNodeByVal(int val);
    void printForward();
    void printBackward();
    Node* getNext(Node* node, bool circular);
    Node* getPrev(Node* node, bool circular);
    Node* getHead();
    Node* getTail();
private:
    Node* head = NULL;
    Node* tail = NULL;

    void addFirstElement(int value);
};

XorList::XorList()
{
}

XorList::~XorList()
{
    while (head != NULL)
    {
        delEnd();
    }
}

Node* XorList::getHead() {
    return head;
}

Node* XorList::getTail() {
    return tail;
}

void XorList::addFirstElement(int value) {
    head = new Node;
    tail = head;

    head->value = value;
    head->prevNext = NULL;
}

Node* XorList::addEnd(int value) {
    if (head == NULL) {
        addFirstElement(value);
    }
    else {
        Node* next = new Node;
        next->value = value;
        next->prevNext = tail;

        tail->prevNext = XOR(next, tail->prevNext);
        tail = next;
    }
    return tail;
}

Node* XorList::addBeg(int value) {
    if (head == NULL) {
        addFirstElement(value);
    }
    else {
        Node* prev = new Node;
        prev->value = value;
        prev->prevNext = head;

        head->prevNext = XOR(prev, head->prevNext);
        head = prev;
    }
    return head;
}

void XorList::insertBefore(Node* node, int val) {
    Node* newNode;
    Node* prev = this->getPrev(node, false);

    if (prev == NULL) {
        addBeg(val);
        return;
    }

    newNode = new Node;
    newNode->value = val;

    prev->prevNext = XOR(node, prev->prevNext);
    prev->prevNext = XOR(newNode, prev->prevNext);

    node->prevNext = XOR(prev, node->prevNext);
    node->prevNext = XOR(newNode, node->prevNext);

    newNode->prevNext = NULL;
    newNode->prevNext = XOR(prev, newNode->prevNext);
    newNode->prevNext = XOR(node, newNode->prevNext);

}

Node* XorList::delBeg() {

    if (head == tail) {
        delete head;
        head = NULL;
        tail = NULL;
    }
    else if (head != NULL) {
        Node* next = getNext(head, false);
        next->prevNext = XOR(head, next->prevNext);
        delete head;
        head = next;
    }

    return head;
}

Node* XorList::delEnd() {
    Node* oldTail = tail;

    if (head == tail) {
        delete head;
        head = NULL;
        tail = NULL;
    }
    else if (tail != NULL) {
        tail = tail->prevNext;
        tail->prevNext = XOR(oldTail, tail->prevNext);
        delete oldTail;
    }

    return tail;
}

void XorList::delNode(Node* node) {
    if (node == head) {
        delBeg();
        return;
    }
    else if (node == tail) {
        delEnd();
        return;
    }

    Node* prev = this->getPrev(node, false);
    Node* next = this->getNext(node, false);

    prev->prevNext = XOR(node, prev->prevNext);
    next->prevNext = XOR(node, next->prevNext);

    prev->prevNext = XOR(next, prev->prevNext);
    next->prevNext = XOR(prev, next->prevNext);

    delete node;
}

void XorList::delNodeByVal(int val) {
    Node* curr = head;
    Node* next;

    while (curr != NULL) {
        next = getNext(curr, false);
        if (curr->value == val) {
            delNode(curr);
        }
        curr = next;
    }
}

Node* XorList::getNext(Node* node, bool circular) {
    Node* curr = head;
    Node* prev = NULL;
    Node* next;

    if (node == tail && circular) {
        return head;
    }

    while (curr != node) {
        if (curr == NULL) {
            return NULL;
        }
        next = XOR(prev, curr->prevNext);
        prev = curr;
        curr = next;
    }

    return XOR(prev, curr->prevNext);
}

Node* XorList::getPrev(Node* node, bool circular) {
    Node* curr = tail;
    Node* next = NULL;
    Node* prev;

    if (node == NULL) {
        return NULL;
    }

    if (node == head  && circular) {
        return tail;
    }

    while (curr != node) {
        if (curr == NULL) {
            return NULL;
        }
        prev = XOR(next, curr->prevNext);
        next = curr;
        curr = prev;
    }

    return XOR(next, curr->prevNext);
}

void XorList::printForward() {
    Node* curr = head;

    if (head == NULL) {
        printf("NULL\n");
        return;
    }

    while (curr != NULL) {
        printf("%d ", curr->value);
        curr = this->getNext(curr, true);

        if (curr == head) {
            break;
        }
    }

    printf("\n");
}

void XorList::printBackward() {
    Node* curr = tail;

    if (head == NULL) {
        printf("NULL\n");
        return;
    }

    while (curr != NULL) {
        printf("%d ", curr->value);
        curr = this->getPrev(curr, false);

        if (curr == tail) {
            break;
        }
    }

    printf("\n");
}

int main()
{
    string command;
    int elementValue;

    XorList list;
    Node* actual = NULL, *tmp = NULL;

    while (cin >> command) {
        if (command == "ACTUAL") {
            if (actual != NULL) {
                printf("%d\n", actual->value);
            }
            else {
                printf("NULL\n");
            }
        }
        else if (command == "NEXT") {
            actual = list.getNext(actual, true);

            if (actual == NULL) {
                printf("NULL\n");
            }
            else {
                printf("%d\n", actual->value);
            }
            
        }
        else if (command == "PREV") {
            actual = list.getPrev(actual, true);
            
            if (actual == NULL) {
                printf("NULL\n");
            }
            else {
                printf("%d\n", actual->value);
            }
        }
        else if (command == "ADD_BEG") {
            scanf_s("%d", &elementValue);
            tmp = list.addBeg(elementValue);

            if (actual == NULL) {
                actual = tmp;
            }
        }
        else if (command == "ADD_END") {
            scanf_s("%d", &elementValue);
            tmp = list.addEnd(elementValue);

            if (actual == NULL) {
                actual = tmp;
            }
        }
        else if (command == "ADD_ACT") {
            scanf_s("%d", &elementValue);
            list.insertBefore(actual, elementValue);
        }
        else if (command == "DEL_BEG") {
            if (actual == list.getHead()) {
                actual = NULL;
            }
            list.delBeg();
        }
        else if (command == "DEL_END") {
            if (actual == list.getTail()) {
                actual = NULL;
            }
            list.delEnd();
        }
        else if (command == "DEL_VAL") {
            scanf_s("%d", &elementValue);

            if (actual != NULL && elementValue == actual->value) {
                actual = NULL;
            }

            list.delNodeByVal(elementValue);
        }
        else if (command == "DEL_ACT") {
            tmp = list.getPrev(actual, true);
            if (actual == tmp) {
                tmp = NULL;
            }

            list.delNode(actual);
            actual = tmp;
        }
        else if (command == "PRINT_FORWARD") {
            list.printForward();
        }
        else if (command == "PRINT_BACKWARD") {
            list.printBackward();
        }
    }
}

