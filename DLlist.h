#ifndef DLLIST_H_INCLUDED
#define DLLIST_H_INCLUDED

//˫������ڵ�ṹ
struct dl_list_node
{
    //������
    int cached_key;    //Flow_ID
    int cached_value;  //counts
    int cached_pkt;
    //ָ����
    dl_list_node * previous;
    dl_list_node * next;
    struct dl_list_head * list_head_p;//point to the list head
};

//˫������ı�ͷ�ṹ
struct dl_list_head
{
    //������
    int list_length;   //������
    //ָ����
    dl_list_node * head;
    dl_list_node * tail;
};

/*��ʼ�������½ڵ���������ָ����*/
void init_dl_list_node(dl_list_node * new_node, int cached_key_1, int cached_value_1, int cached_pkt_1, dl_list_head * list_head_p_temp);

/*���뵽������ǰ��*/
void insert_node_head(dl_list_head * list_head, dl_list_node * new_node);

/*ɾ������ĩβ�Ľڵ�*/
int delete_node_tail(dl_list_head * list_head);

/*����ָ�룬ɾ������ĳ�ڵ�*/
void delete_node_pointed(dl_list_head * list_head_p, dl_list_node * list_ptr);

/*���뵽����ĳ�ڵ�ǰ��*/
void insert_data_before(dl_list_node * old_node, dl_list_node * new_node);





#endif // DLLIST_H_INCLUDED




