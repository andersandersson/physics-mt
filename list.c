#include "list.h"

#include <stdlib.h>

Node* node_create() {
  Node* node = malloc(sizeof(Node));

  node->item = NULL;
  node->next = NULL;

  return node;
}

void node_free(Node* node) {
  free(node->item);
  free(node);
}

List* list_create() {
  List* new_list;

  new_list = malloc(sizeof(List));

  new_list->first_node = NULL;
  new_list->last_node = NULL;

  return new_list;
}

void list_free(List* list) {
  Node* node = list->first_node;
  Node* next_node;

  while(NULL != node) {
    next_node = node->next;

    node_free(node);

    node = next_node;
  }

  free(list);
}

void list_clear(List* list) {
  Node* node = list->first_node;
  Node* next_node;

  list->first_node = NULL;
  list->last_node = NULL;

  while(NULL != node) {
    next_node = node->next;

    free(node);

    node = next_node;
  }
}

void list_push(List* list, void* item) {
  Node* node = node_create();
  
  node->next = list->first_node;
  node->item = item;

  if(NULL == list->first_node) {
    list->last_node = node;
  }

  list->first_node = node;
}

void* list_pop(List* list) {
  Node* node = list->first_node;
  void* item = NULL;

  if(NULL != node) {
    list->first_node = node->next;

    item = node->item;

    free(node);
  }

  if(NULL == list->first_node) {
    list->last_node = NULL;
  }

  return item;
}

