#include "allmyfile.h"

/*initialize a dll-type node*/
void init_dl_list_node(dl_list_node * new_node, int cached_key_1, int cached_value_1, int cached_pkt_1, dl_list_head * list_head_p_temp)
{
    new_node->next         = NULL;
    new_node->previous     = NULL;
    new_node->cached_key   = cached_key_1;
    new_node->cached_value = cached_value_1;
    new_node->cached_pkt = cached_pkt_1;
    new_node->list_head_p  = list_head_p_temp;
    return;
}


/*insert a node to the head of the list*/
void insert_node_head(dl_list_head * list_head, dl_list_node * new_node)
{
    if(list_head->tail == NULL)
    {
        //if a list is empty, insert a new node
        list_head->head = list_head->tail = new_node;
    }
    else
    {
        new_node->next = list_head->head;
        new_node->next->previous = new_node;
        list_head->head = new_node;
    }

    list_head->list_length ++;

    return;
}

/*delete the node in the tail*/
int delete_node_tail(dl_list_head * list_head)
{
    dl_list_node * temp_node;
    temp_node = list_head->tail;
    list_head->tail = temp_node->previous;
    list_head->tail->next = NULL;
    int i = temp_node->cached_key;
    delete temp_node;

    list_head->list_length --;

    return i;
}

/*delete a node in the list*/
void delete_node_pointed(dl_list_head * list_head_p, dl_list_node * list_ptr)
{
    if(list_ptr->next == NULL && list_ptr->previous == NULL)//if a list only have one element
    {
        delete list_ptr;
        list_head_p->head = NULL;
        list_head_p->tail = NULL;
    }
    else if(list_ptr->next == NULL)//delete the tail of the list
    {
        list_ptr->previous->next = NULL;
        list_head_p->tail = list_ptr->previous;
        delete list_ptr;
    }
    else if(list_ptr->previous == NULL)//delete the head of the list
    {
        list_ptr->next->previous = NULL;
        list_head_p->head = list_ptr->next;
        delete list_ptr;
    }
    else
    {
        list_ptr->previous->next = list_ptr->next;
        list_ptr->next->previous = list_ptr->previous;
        delete list_ptr;
    }

    list_head_p->list_length --;

    return;
}

/*insert a new node before an old node*/
void insert_data_before(dl_list_node * old_node, dl_list_node * new_node)
{
    return;
}
