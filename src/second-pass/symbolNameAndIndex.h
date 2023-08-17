#include <stdlib.h>
#include <string.h>

struct SymbolNameAndIndex {
    char *name;
    int IC_index;
    struct SymbolNameAndIndex* next;
};

// Function to create a new node */
struct SymbolNameAndIndex* createNode(const char *name, int IC_index, int line_number) {
    struct SymbolNameAndIndex* newNode = (struct SymbolNameAndIndex *)malloc(sizeof(struct SymbolNameAndIndex));
    if (newNode == NULL) {
       /*  handle_error(FailedToAllocateMemory, line_number); */
        return NULL;
    }

    newNode->name = strdup(name);  // Duplicate the string to avoid issues */
    newNode->IC_index = IC_index;
    newNode->next = NULL;
    return newNode;
}

/* Function to insert a node at the end of the list */
void insertNode(struct SymbolNameAndIndex** head, struct SymbolNameAndIndex* newNode) {
    if (*head == NULL) {
        *head = newNode;
        return;
    }

    struct SymbolNameAndIndex* current = *head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = newNode;
}


/* Function to free the linked list and its nodes */
void freeList(struct SymbolNameAndIndex* head) {
    struct SymbolNameAndIndex* current = head;
    while (current != NULL) {
        struct SymbolNameAndIndex* temp = current;
        current = current->next;
        free(temp->name);
        free(temp);
    }
}


/* Function to find a node by name */
struct SymbolNameAndIndex* getNodeByName(const struct SymbolNameAndIndex* head, const char* targetName) {
    struct SymbolNameAndIndex* current = (struct SymbolNameAndIndex *)head;
    while (current != NULL) {
        if (strcmp(current->name, targetName) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

/* Function to get the next index */
int getNextIndex(const struct SymbolNameAndIndex* head, int currentIndex) {
    const struct SymbolNameAndIndex* current = head;
    int nextIndex = -1;  // Default value if not found */
    while (current != NULL) {
        if (current->IC_index > currentIndex && (nextIndex == -1 || current->IC_index < nextIndex)) {
            nextIndex = current->IC_index;
        }
        current = current->next;
    }
    return nextIndex;
}

/* Function to get the next name */
const char* getNextName(const struct SymbolNameAndIndex* head, const char* currentName) {
    const struct SymbolNameAndIndex* current = head;
    const char* nextName = NULL;  /* Default value if not found */
    while (current != NULL) {
        if (strcmp(current->name, currentName) > 0 && (nextName == NULL || strcmp(current->name, nextName) < 0)) {
            nextName = current->name;
        }
        current = current->next;
    }
    return nextName;
}