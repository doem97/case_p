#include "allmyfile.h"

/*��ʼ�������½ڵ���������ָ����*/
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


/*���뵽������ǰ��*/
void insert_node_head(dl_list_head * list_head, dl_list_node * new_node)
{
    //list_head��ʾҪ���������ı�ͷ��insert_nodeָ��Ҫ����Ľڵ�
    if(list_head->tail == NULL)
    {
        //����Ϊ�գ������һ���ڵ�
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

/*ɾ������ĩβ�Ľڵ㣬ֻ����cache��ʱ��,cache size >> 1*/
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

/*����ָ�룬ɾ������ĳ�ڵ�*/
void delete_node_pointed(dl_list_head * list_head_p, dl_list_node * list_ptr)
{
    if(list_ptr->next == NULL && list_ptr->previous == NULL)//����ֻ��һ��Ԫ��
    {
        delete list_ptr;
        list_head_p->head = NULL;
        list_head_p->tail = NULL;
    }
    else if(list_ptr->next == NULL)//ɾ������β��Ԫ��
    {
        list_ptr->previous->next = NULL;
        list_head_p->tail = list_ptr->previous;
        delete list_ptr;
    }
    else if(list_ptr->previous == NULL)//ɾ������ͷ��Ԫ��
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

/*���뵽����ĳ�ڵ�ǰ��*/
void insert_data_before(dl_list_node * old_node, dl_list_node * new_node)
{
    //old_nodeָ�����е�ĳ�ڵ㣬new_nodeָ�ı�����Ľڵ�
    return;
}
