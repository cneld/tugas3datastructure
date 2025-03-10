#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TABLE_SIZE 10

struct node {
    char data[100];
    struct node *next;
} *hashTable[TABLE_SIZE], *curr;

char linearProbingTable[TABLE_SIZE][100];
char rehashingTable[TABLE_SIZE][100];

int hashFunctionType = 1;
int collisionHandlingType = 1;

void initializeHashTable() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        hashTable[i] = NULL;
        linearProbingTable[i][0] = '\0';
        rehashingTable[i][0] = '\0';
    }
}

void freeHashTable() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        struct node *temp;
        while (hashTable[i] != NULL) {
            temp = hashTable[i];
            hashTable[i] = hashTable[i]->next;
            free(temp);
        }
    }
}

int divisionHash(int key) { return key % TABLE_SIZE; }
int midSquareHash(int key) { int squared = key * key; return (squared / 10) % TABLE_SIZE; }
int foldingHash(int key) { return (key / 1000 + key % 1000) % TABLE_SIZE; }
int digitExtractionHash(int key) { return (key / 10) % TABLE_SIZE; }
int rotationHash(int key) { return ((key % 10) * 10000 + key / 10) % TABLE_SIZE; }

int getHashKey(char data[]) {
    int key = tolower(data[0]) - 'a';
    switch (hashFunctionType) {
        case 1: return divisionHash(key);
        case 2: return midSquareHash(key);
        case 3: return foldingHash(key);
        case 4: return digitExtractionHash(key);
        case 5: return rotationHash(key);
        default: return divisionHash(key);
    }
}

void pushBack(int hashKey, char data[]) {
    struct node *newNode = (struct node *)malloc(sizeof(struct node));
    if (!newNode) return;
    strcpy(newNode->data, data);
    newNode->next = NULL;
    
    if (collisionHandlingType == 1) {
        if (hashTable[hashKey] == NULL) {
            hashTable[hashKey] = newNode;
        } else {
            curr = hashTable[hashKey];
            while (curr->next != NULL) curr = curr->next;
            curr->next = newNode;
        }
    } else if (collisionHandlingType == 2) {
        int index = hashKey;
        while (linearProbingTable[index][0] != '\0') index = (index + 1) % TABLE_SIZE;
        strcpy(linearProbingTable[index], data);
    } else if (collisionHandlingType == 3) {
        int index = hashKey, step = 1;
        while (rehashingTable[index][0] != '\0') index = (index + step * step) % TABLE_SIZE, step++;
        strcpy(rehashingTable[index], data);
    }
}

void deleteData(char data[]) {
    int hashKey = getHashKey(data);
    struct node *temp = hashTable[hashKey], *prev = NULL;
    while (temp != NULL && strcmp(temp->data, data) != 0) prev = temp, temp = temp->next;
    if (temp != NULL) {
        if (prev == NULL) hashTable[hashKey] = temp->next;
        else prev->next = temp->next;
        free(temp);
        printf("Data berhasil dihapus!\n");
        return;
    }
    for (int i = 0; i < TABLE_SIZE; i++) {
        if (strcmp(linearProbingTable[i], data) == 0) { linearProbingTable[i][0] = '\0'; printf("Data berhasil dihapus!\n"); return; }
        if (strcmp(rehashingTable[i], data) == 0) { rehashingTable[i][0] = '\0'; printf("Data berhasil dihapus!\n"); return; }
    }
    printf("Data tidak ditemukan!\n");
}

void viewHashTable() {
    if (collisionHandlingType == 1) {
        printf("\nChaining Method:\n");
        for (int i = 0; i < TABLE_SIZE; i++) {
            if (hashTable[i] != NULL) {
                printf("Index [%d]: ", i);
                curr = hashTable[i];
                while (curr != NULL) { printf("%s", curr->data); if (curr->next != NULL) printf(" -> "); curr = curr->next; }
                printf("\n");
            }
        }
    } else if (collisionHandlingType == 2) {
        printf("\nLinear Probing Method:\n");
        for (int i = 0; i < TABLE_SIZE; i++) printf("Index [%d]: %s\n", i, linearProbingTable[i][0] ? linearProbingTable[i] : "(empty)");
    } else if (collisionHandlingType == 3) {
        printf("\nRehashing Method:\n");
        for (int i = 0; i < TABLE_SIZE; i++) printf("Index [%d]: %s\n", i, rehashingTable[i][0] ? rehashingTable[i] : "(empty)");
    }
}

void menu() {
    int choice;
    char data[100];
    do {
        printf("\n1. Choose Hash Function\n2. Choose Collision Handling\n3. Insert Data\n4. Delete Data\n5. View Hash Table\n6. Exit\nEnter choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1: printf("Choose Hash Function (1. Division, 2. Mid-Square, 3. Folding, 4. Digit Extraction, 5. Rotation): "); scanf("%d", &hashFunctionType); break;
            case 2: printf("Choose Collision Handling (1. Chaining, 2. Linear Probing, 3. Rehashing): "); scanf("%d", &collisionHandlingType); break;
            case 3: printf("Enter data: "); scanf("%s", data); pushBack(getHashKey(data), data); break;
            case 4: printf("Enter data to delete: "); scanf("%s", data); deleteData(data); break;
            case 5: viewHashTable(); break;
            case 6: freeHashTable(); printf("Exiting...\n"); break;
            default: printf("Invalid choice!\n");
        }
    } while (choice != 6);
}

int main() {
    initializeHashTable();
    menu();
    return 0;
}