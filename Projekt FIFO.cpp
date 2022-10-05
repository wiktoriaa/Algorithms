#include <iostream>
#include <list>
#include <string>
using namespace std;

struct Element {
    Element* prev;
    Element* next;
    int key;
    Element(Element* p, Element* e, int k) : prev(p), next(e), key(k) { }
};

class LinkedList {
public:
    ~LinkedList();
    Element* push(Element* element, int value);
    int getSize() const;
    void print() const;
    void zeroElements(Element* beg, Element* end) const;
    void deleteElements(Element* beg, Element* end);
    void printBackward() const;
    void addBeg(int value);
    void addEnd(int value);
    void delBeg();
    void delEnd();
    Element* getTail();
private:
    Element* head = NULL;
    int size = 0;

    Element* pushBack(int value);
    void deleteAll();
};

int LinkedList::getSize() const {
    return size;
}

void LinkedList::deleteAll() {
    Element* tmp;

    for (int i = 0; i < size; i++) {
        tmp = head->next;
        delete head;
        head = tmp;
        size--;
    }
    head = NULL;
}

LinkedList::~LinkedList() {
    deleteAll();
}

Element* LinkedList::push(Element* element, int value) {
    Element* elm;

    if (head == NULL) {
        head = new Element(NULL, NULL, value);
        head->next = head;
        head->prev = head;
        size++;
        return head;
    }
    else if (element == head) {
        addBeg(value);
        return head;
    }
    else {
        elm = new Element(element->prev, element, value);
        element->prev->next = elm;
        element->prev = elm;
        size++;
        return elm;
    }
}

void LinkedList::printBackward() const {
    Element* curr = head;
    if (!head) {
        printf("NULL\n");
        return;
    }

    for (int i = 0; i < size; i++) {
        curr = curr->prev;
        printf("%d ", curr->key);
    }
    printf("\n");
}

void LinkedList::print() const {
    Element* curr = head;
    if (!head) {
        printf("NULL\n");
        return;
    }

    for (int i = 0; i < size; i++) {
        printf("%d ", curr->key);
        curr = curr->next;
    }
    printf("\n");
}

void LinkedList::zeroElements(Element* beg, Element* end) const {
    if (beg == NULL || end == NULL) {
        if (!head) return;
        beg = head;
        end = head->next;
        beg->key = 0;
        end->key = 0;
    }

    beg = beg->next;
    while (beg != end)
    {
        beg->key = 0;
        beg = beg->next;
    }
}

void LinkedList::deleteElements(Element* beg, Element* end) {
    if (beg == NULL || end == NULL) {
        deleteAll();
        return;
    }

    Element* tmp;

    beg = beg->next;
    while (beg != end) {
        beg->prev->next = beg->next;
        beg->next->prev = beg->prev;
        tmp = beg->next;
        delete beg;
        beg = tmp;
        size--;
    }
    
}

Element* LinkedList::pushBack(int value) {
    Element* tmp;

    if (!head) {
        return push(NULL, value);
    }
    else {
        tmp = new Element(head->prev, head, value);
        head->prev->next = tmp;
        head->prev = tmp;
    }
    size++;
    return head->prev;
}

void LinkedList::addBeg(int value) {
    head = pushBack(value);
}

void LinkedList::addEnd(int value) {
    pushBack(value);
}

void LinkedList::delBeg() {

    if (size == 1) {
        size = 0;
        delete head;
        head = NULL;
        return;
    }

    Element* tmp;
    head->prev->next = head->next;
    head->next->prev = head->prev;
    tmp = head->next;
    delete head;
    head = tmp;

    size--;
}

void LinkedList::delEnd() {

    if (size == 1) {
        size = 0;
        delete head;
        head = NULL;
        return;
    }

    Element* oldTail = head->prev;
    head->prev = head->prev->prev;
    head->prev->next = head;
    delete oldTail;

    size--;
}

Element* LinkedList::getTail() {
    return head->prev;
}


class FIFO {
public:
    size_t getListSize() const;
    int getCount() const;
    void push(int value);
    void pop();
    void printQueue();
    void garbageSoft() const;
    void garbageHard();
    void printForwardList() const;
    void printBackwardList() const;
    void addBegList(int value);
    void addEndList(int value);
    void delBegList();
    void delEndList();

private:
    LinkedList list;
    Element* first = NULL;
    Element* last = NULL;
    bool isEmpty = true;
};

size_t FIFO::getListSize() const {
    return list.getSize();
}

void FIFO::push(int value) {
    if (!first) {
        if (getListSize() == 0) {
            first = list.push(NULL, value);
        }
        else {
            first = list.getTail();
            first->key = value;
        }
        last = first;
        isEmpty = false;
        return;
    }

    if (isEmpty) {
        first->key = value;
        isEmpty = false;
        return;
    }

    if (last->prev == first) {
        last->prev = list.push(last, value);
        last = last->prev;
    }
    else
    {
        last = last->prev;
        last->key = value;
    }
}

void FIFO::pop() {
    if (isEmpty) {
        printf("NULL\n");
        return;
    }

    printf("%d\n", first->key);

    if (first == last) {
        isEmpty = true;
        return;
    }
    first = first->prev;
}

void FIFO::printQueue() {
    Element* curr = first;

    if (isEmpty) {
        printf("NULL\n");
        return;
    }

    while (curr != last) {
        printf("%d ", curr->key);
        curr = curr->prev;
    }
    printf("%d ", last->key);
    printf("\n");
}

int FIFO::getCount() const {
    if (isEmpty) return 0;

    if (first == last) return 1;

    Element *curr = first;
    int count = 0;

    while (curr != last) {
        count++;
        curr = curr->prev;
    }
    count++;

    return count;
}

void FIFO::garbageSoft() const {
    list.zeroElements(first, last);
}

void FIFO::garbageHard() {
    list.deleteElements(first, last);
}

void FIFO::printForwardList() const {
    list.print();
}

void FIFO::printBackwardList() const {
    list.printBackward();
}

void FIFO::addBegList(int value) {
    list.addBeg(value);
}

void FIFO::addEndList(int value) {
    list.addEnd(value);
}

void FIFO::delBegList() {
    if (list.getTail()->next == first) {
        pop();
    }
    else if (list.getTail()->next == last) {
        last = last->next;
    }

    list.delBeg();
}

void FIFO::delEndList() {
    if (list.getTail() == first) {
        pop();
    }
    else if (list.getTail() == last) {
        last = last->next;
    }

    list.delEnd();
}

int main()
{
    FIFO fifo;
    int key;
    string command;

    while (cin >> command) {
        if (command == "PUSH") {
            scanf_s("%d", &key);
            fifo.push(key);
        }
        else if (command == "POP") {
            fifo.pop();
        }
        else if (command == "PRINT_QUEUE") {
            fifo.printQueue();
        }
        else if (command == "COUNT") {
            printf("%d\n", fifo.getCount());
        }
        else if (command == "GARBAGE_SOFT") {
            fifo.garbageSoft();
        }
        else if (command == "GARBAGE_HARD") {
            fifo.garbageHard();
        }
        else if (command == "SIZE") {
            printf("%zu\n", fifo.getListSize());
        }

        /* stare funkcje */
        else if (command == "ADD_BEG") {
            scanf_s("%d", &key);
            fifo.addBegList(key);
        }
        else if (command == "ADD_END") {
            scanf_s("%d", &key);
            fifo.addEndList(key);
        }
        else if (command == "DEL_BEG") {
            fifo.delBegList();
        }
        else if (command == "DEL_END") {
            fifo.delEndList();
        }
        else if (command == "PRINT_FORWARD") {
            fifo.printForwardList();
        }
        else if (command == "PRINT_BACKWARD") {
            fifo.printBackwardList();
        }
    }
}
