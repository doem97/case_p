#include "allmyfile.h"

/*初始化链表新节点的数据域和指针域*/
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


/*插入到链表最前面*/
void insert_node_head(dl_list_head * list_head, dl_list_node * new_node)
{
    //list_head表示要插入的链表的表头，insert_node指向要插入的节点
    if(list_head->tail == NULL)
    {
        //链表为空，加入第一个节点
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

/*删除链表末尾的节点，只能在cache满时用,cache size >> 1*/
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

/*给出指针，删除链表某节点*/
void delete_node_pointed(dl_list_head * list_head_p, dl_list_node * list_ptr)
{
    if(list_ptr->next == NULL && list_ptr->previous == NULL)//链表只有一个元素
    {
        delete list_ptr;
        list_head_p->head = NULL;
        list_head_p->tail = NULL;
    }
    else if(list_ptr->next == NULL)//删除链表尾部元素
    {
        list_ptr->previous->next = NULL;
        list_head_p->tail = list_ptr->previous;
        delete list_ptr;
    }
    else if(list_ptr->previous == NULL)//删除链表头部元素
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

/*插入到链表某节点前面*/
void insert_data_before(dl_list_node * old_node, dl_list_node * new_node)
{
    //old_node指的已有的某节点，new_node指的被插入的节点
    return;
}
