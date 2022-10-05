#include <iostream>
using namespace std;

void debugArray(int n, int* tab) {
    for (int i = 0; i < n; i++) {
        printf("%d ", tab[i]);
    }
    printf("\n");
}

void heapify(int* arr, int n, int i)
{
    int largest = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;

    if (l < n && arr[l] > arr[largest])
        largest = l;

    if (r < n && arr[r] > arr[largest])
        largest = r;

    if (largest != i) {
        swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

int main()
{
    int n;
    int* tab;

    while (cin >> n) {
        tab = new int[n];
        for (int i = 0; i < n; i++) {
            scanf_s("%d", &tab[i]);
        }

        for (int i = n / 2 - 1; i >= 0; i--) {
            heapify(tab, n, i);
            debugArray(n, tab);
        }

        delete[] tab;
    }
}
