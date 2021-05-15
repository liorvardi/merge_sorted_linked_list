#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <time.h>

//
// Created by lior1 on 5/6/2021.
//

typedef struct node_t {
    int x;
    struct node_t *next;
} *Node;

typedef enum {
    SUCCESS=0,
    MEMORY_ERROR,
    UNSORTED_LIST,
    NULL_ARGUMENT,
} ErrorCode;


bool isListSorted(Node list) {
    return true;
}

void destroyList(Node list) {
    while (list != NULL){
        Node destroy_next = list->next;
        free(list);
        list = destroy_next;
    }
}

ErrorCode copy_list(Node source, Node dest) {
    if(dest == NULL){
        return NULL_ARGUMENT;
    }
    while (source->next != NULL) {
        dest->x = source->x;
        dest->next = malloc(sizeof(struct node_t));
        if ((dest->next) == NULL) {
            return MEMORY_ERROR;
        }
        dest = dest->next;
        source = source->next;
    }
    dest->x = source->x;
    dest->next = NULL;
    return SUCCESS;
}

Node mergeSortedLists(Node list1, Node list2, ErrorCode* error_code) {
    if(error_code == NULL){
        return NULL;
    }
    if (list1 == NULL || list2 == NULL) {
        *error_code = NULL_ARGUMENT;
        return NULL;
    }
    if(isListSorted(list1) == false || isListSorted(list2) == false){
        *error_code = UNSORTED_LIST;
        return NULL;
    }
    Node first_node = malloc(sizeof(struct node_t));
    if ((first_node) == NULL) {
        *error_code = MEMORY_ERROR;
        return NULL;
    }
    Node merged_list;
    for(merged_list = first_node; list1 != NULL && list2 != NULL; merged_list = merged_list->next){
        merged_list->next = malloc(sizeof(struct node_t));
        if (NULL == (merged_list->next)) {
            destroyList(first_node);
            *error_code = MEMORY_ERROR;
            return NULL;
        }
        int x1 = list1->x;
        int x2 = list2->x;
        if (x1 < x2) {
            merged_list->x = x1;
            list1 = list1->next;
        } else {
            merged_list->x = x2;
            list2 = list2->next;
        }
    }
    ErrorCode list_copy_error_code;
    if (list1 == NULL) {
        list_copy_error_code = copy_list(list2, merged_list);
    } else {
        list_copy_error_code = copy_list(list1, merged_list);
    }
    if (list_copy_error_code == MEMORY_ERROR) {
        *error_code = MEMORY_ERROR;
        return NULL;
    }
    assert(list_copy_error_code != NULL_ARGUMENT);
    return first_node;
}


void printList(Node list){
    for(; list != NULL; list = list->next){
        printf("%d ", list->x);
    }
    printf("\n");
}


int main() {
    Node list1 = malloc(sizeof(struct node_t));
    Node list2 = malloc(sizeof(struct node_t));
    Node first1 = list1;
    Node first2 = list2;
    list1->x = 4;
    list2->x = 2;
    for(int i = 0; i < 5; i++) {
        list1->next = malloc(sizeof(*list1));
        list2->next = malloc(sizeof(struct node_t));
        list1->next->x=list1->x + 1;
        list2->next->x = list2->x +2;
        list1 = list1->next;
        list2 = list2->next;
    }
    list1->next = NULL;
    list2->next = NULL;
    printList(first1);
    printList(first2);
    ErrorCode error;
    Node merged = mergeSortedLists(first1, first2, &error);
    printList(merged);
    destroyList(first1);
    destroyList(first2);
    destroyList(merged);
    return 0;
}
