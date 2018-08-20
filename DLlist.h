#ifndef DLLIST_H_INCLUDED
#define DLLIST_H_INCLUDED

//双向链表节点结构
struct dl_list_node
{
    //数据域
    int cached_key;    //Flow_ID
    int cached_value;  //counts
    int cached_pkt;
    //指针域
    dl_list_node * previous;
    dl_list_node * next;
    struct dl_list_head * list_head_p;//point to the list head
};

//双向链表的表头结构
struct dl_list_head
{
    //数据域
    int list_length;   //链表长度
    //指针域
    dl_list_node * head;
    dl_list_node * tail;
};

/*初始化链表新节点的数据域和指针域*/
void init_dl_list_node(dl_list_node * new_node, int cached_key_1, int cached_value_1, int cached_pkt_1, dl_list_head * list_head_p_temp);

/*插入到链表最前面*/
void insert_node_head(dl_list_head * list_head, dl_list_node * new_node);

/*删除链表末尾的节点*/
int delete_node_tail(dl_list_head * list_head);

/*给出指针，删除链表某节点*/
void delete_node_pointed(dl_list_head * list_head_p, dl_list_node * list_ptr);

/*插入到链表某节点前面*/
void insert_data_before(dl_list_node * old_node, dl_list_node * new_node);





#endif // DLLIST_H_INCLUDED




