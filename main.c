#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
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

int getListLength(Node list) {
    return 1;
}

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
    while (source != NULL) {
        dest->next = malloc(sizeof(struct node_t));
        if ((dest->next) == NULL) {
            return MEMORY_ERROR;
        }
        dest = dest->next;
        dest->x = source->x;
        source = source->next;
    }
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
    Node demi_node = malloc(sizeof(struct node_t));
    if ((demi_node) == NULL) {
        *error_code = MEMORY_ERROR;
        return NULL;
    }
    Node merged_list;
    for(merged_list = demi_node; list1 != NULL && list2 != NULL; merged_list = merged_list->next){
        merged_list->next = malloc(sizeof(struct node_t));
        if (NULL == (merged_list->next)) {
            destroyList(demi_node);
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
        destroyList(demi_node);
        *error_code = MEMORY_ERROR;
        return NULL;
    }
    assert(list_copy_error_code != NULL_ARGUMENT);
    Node first_node = demi_node->next;
    free(demi_node);
    return first_node;
}

int main() {
    Node list1 = malloc(sizeof(struct node_t));
    Node list2 = malloc(sizeof(struct node_t));
    ErrorCode error_code;
    error_code = SUCCESS;
    list1->x = 2;
    list1->next = NULL;
    list2->x = 1;
    list2->next = NULL;
    Node merged = mergeSortedLists(list1, list2, &error_code);
    printf("error code = %d\n2nd number = %d", error_code, merged->next->x);
    return 0;
}
