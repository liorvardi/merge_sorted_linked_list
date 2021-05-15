#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

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

int getListLength(Node list)
{
    int cnt = 0;
    while(list != NULL)
    {
        cnt++;
        list = list->next;
    }
    return cnt;
}

bool isListSorted(Node list)
{
    return true;
}

// my functions
int getNextValue(Node *list1, Node *list2)
{
    int value;
    if(*list1 == NULL)
    {
        value = (*list2)->x;
        *list2 = (*list2)->next;
    }
    else if(*list2 == NULL)
    {
        value = (*list1)->x;
        *list1 = (*list1)->next;
    }
    else if((*list1)->x <= (*list2)->x)
    {
        value = (*list1)->x;
        *list1 = (*list1)->next;
    }
    else
    {
        value = (*list2)->x;
        *list2 = (*list2)->next;
    }

    return value;
}

void cleanup(Node node)
{
    Node tmp;
    while(node != NULL)
    {
        tmp = node->next;
        free(node);
        node = tmp;
    }
}

Node mergeSortedLists(Node list1, Node list2, ErrorCode* error_code)
{
    int size1 = getListLength(list1);
    int size2 = getListLength(list2);

    if(error_code == NULL)
    {
        return NULL;
    }

    if(!size1 || !size2)
    {
        *error_code = NULL_ARGUMENT;
        return NULL;
    }
    if(!isListSorted(list1) || !isListSorted(list2))
    {
        *error_code = UNSORTED_LIST;
        return NULL;
    }

    Node first = (Node)malloc(sizeof(struct node_t));
    if(first == NULL)
    {
        *error_code = MEMORY_ERROR;
        return NULL;
    }

    Node current_node = first;
    Node next_node;
    for (int i = 0; i < size1+size2-1; ++i)
    {
        current_node->x = getNextValue(&list1, &list2);
        next_node = (Node)malloc(sizeof(struct node_t));
        if(next_node == NULL)
        {
            *error_code = MEMORY_ERROR;
            cleanup(first);
            return NULL;
        }
        current_node->next = next_node;
        current_node = next_node;
    }

    current_node->x = getNextValue(&list1, &list2);
    current_node->next = NULL;
    *error_code = SUCCESS;
    return first;
}


// tests
void sanity_test()
{
    ErrorCode error_code;

    struct node_t left, left2, left3;
    left.x = 1; left2.x = 4; left3.x = 9;
    left.next = &left2; left2.next = &left3, left3.next = NULL;

    struct node_t right, right2, right3;

    right.x = 2; right2.x = 4; right3.x = 8;
    right.next = &right2; right2.next = &right3, right3.next = NULL;

    Node result = mergeSortedLists(&left, &right, &error_code);

    Node first = result;
    assert (result->x == 1);
    result = result->next;
    assert (result->x == 2);
    result = result->next;
    assert (result->x == 4);
    result = result->next;
    assert (result->x == 4);
    result = result->next;
    assert (result->x == 8);
    result = result->next;
    assert (result->x == 9);

    cleanup(first);
}

void errors_tests()
{
    ErrorCode error_code;

    Node list1 = NULL;
    struct node_t list2;
    list2.x = 2;
    list2.next = NULL;

    Node result;
    result = mergeSortedLists(list1, &list2, &error_code);
    assert (error_code ==  NULL_ARGUMENT);

    cleanup(result);

    result = mergeSortedLists(&list2, list1, &error_code);
    assert (error_code ==  NULL_ARGUMENT);
    cleanup(result);

    ErrorCode *error_code2 = NULL;
    result = mergeSortedLists(&list2, list1, error_code2);
    assert (result == NULL);
    cleanup(result);
}

int main()
{
    sanity_test();
    errors_tests();
    return 0;
}
