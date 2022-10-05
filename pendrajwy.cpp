#include <iostream>
#include <stdio.h>
using namespace std;

struct FileInfo {
    int id;
    int size;
};

struct FilesList {
    FileInfo* file;
    FilesList* next;
};

void SwapElements(FileInfo& x, FileInfo& y) {
    FileInfo tmp;
    tmp = x;
    x = y;
    y = tmp;
}

void QuickSort(FileInfo* files, int left, int right) {

    if (left < right) {
        int j = left;
        for (int i = left + 1; i <= right; i++) {
            if (files[i].size < files[left].size) {
                SwapElements(files[++j], files[i]);
            }
        }
        SwapElements(files[left], files[j]);

        QuickSort(files, left, j - 1);
        QuickSort(files, j + 1, right);
    }
    
}


void QuickSortId(FileInfo* files, int left, int right) {

    if (left < right) {
        int j = left;
        for (int i = left + 1; i <= right; i++) {
            if (files[i].id > files[left].id) {
                SwapElements(files[++j], files[i]);
            }
        }
        SwapElements(files[left], files[j]);

        QuickSortId(files, left, j - 1);
        QuickSortId(files, j + 1, right);
    }
}

int main()
{
    ios_base::sync_with_stdio(false);
    int filesCount, pendriveSize;

    scanf_s("%d", &filesCount);
    scanf_s("%d", &pendriveSize);

    FileInfo* files = new FileInfo[filesCount];

    for (int i = 0; i < filesCount; i++) {
        scanf_s("%d", &files[i].id);
        scanf_s("%d", &files[i].size);
    }

    QuickSort(files, 0, filesCount - 1);

    FilesList* pendriveOne = new FilesList;
    FilesList* pendriveTwo = new FilesList;
    pendriveOne->next = NULL;
    pendriveTwo->next = NULL;

    FilesList* CurrentOne = pendriveOne;
    FilesList* CurrentTwo = pendriveTwo;

    int pendriveOneMem = pendriveSize;
    int pendriveTwoMem = pendriveSize;

    int copiedFilesOneCount = 0;
    int copiedFilesTwoCount = 0;

    for (int i = 0; i < filesCount; i++) {
        if (pendriveOneMem >= files[i].size) {
            CurrentOne->file = &files[i];

            CurrentOne->next = new FilesList;
            CurrentOne = CurrentOne->next;
            CurrentOne->next = NULL;

            pendriveOneMem -= files[i].size;
            copiedFilesOneCount++;
        }
        else if (pendriveTwoMem >= files[i].size) {
            CurrentTwo->file = &files[i];

            CurrentTwo->next = new FilesList;
            CurrentTwo = CurrentTwo->next;
            CurrentTwo->next = NULL;

            pendriveTwoMem -= files[i].size;
            copiedFilesTwoCount++;
        }
        else {
            break;
        }
    }

    printf("%d\n", copiedFilesOneCount + copiedFilesTwoCount);

    FileInfo* filesOnPedriveOne = new FileInfo[copiedFilesOneCount];
    FileInfo* filesOnPedriveTwo = new FileInfo[copiedFilesTwoCount];
    CurrentOne = pendriveOne;
    CurrentTwo = pendriveTwo;
    FilesList* next;

    printf("1: ");

    if (copiedFilesOneCount > 0) {
        for (int i = 0; i < copiedFilesOneCount; i++) {
            next = CurrentOne->next;
            filesOnPedriveOne[i] = *CurrentOne->file;

            CurrentOne->next = NULL;
            free(CurrentOne);

            CurrentOne = next;
        }

        QuickSortId(filesOnPedriveOne, 0, copiedFilesOneCount - 1);

        for (int i = 0; i < copiedFilesOneCount; i++) {
            printf("%d ", filesOnPedriveOne[i].id);
        }
    }
    printf("\n");

    cout << "2: ";
    if (copiedFilesTwoCount > 0) {
        for (int i = 0; i < copiedFilesTwoCount; i++) {
            next = CurrentTwo->next;
            filesOnPedriveTwo[i] = *CurrentTwo->file;

            CurrentTwo->next = NULL;
            free(CurrentTwo);

            CurrentTwo = next;
        }

        QuickSortId(filesOnPedriveTwo, 0, copiedFilesTwoCount - 1);

        for (int i = 0; i < copiedFilesTwoCount; i++) {
            printf("%d ", filesOnPedriveTwo[i].id);
        }
    }
    printf("\n");

    pendriveOne = NULL;
    pendriveTwo = NULL;
    
    delete[] filesOnPedriveOne;
    delete[] filesOnPedriveTwo;
}

