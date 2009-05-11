#ifndef LIST_H
#define LIST_H

typedef struct Node {
  void* item;
  struct Node* next;
} Node;

typedef struct List {
  Node* first_node;
  Node* last_node;
} List;

Node* node_create();
void  node_free(Node* node);

List* list_create();
void  list_free(List* list);
void  list_clear(List* list);
void  list_push(List* list, void* item);
void* list_pop(List* list);

#endif
