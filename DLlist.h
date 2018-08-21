#ifndef DLLIST_H_INCLUDED
#define DLLIST_H_INCLUDED

//double linked list node structure
struct dl_list_node
{
    //data field
    int cached_key;    //Flow_ID
    int cached_value;  //counts
    int cached_pkt;
    //pointer field
    dl_list_node * previous;
    dl_list_node * next;
    struct dl_list_head * list_head_p;//point to the list head
};

//list head of double linked list
struct dl_list_head
{
    //data field
    int list_length;
    //pointer field
    dl_list_node * head;
    dl_list_node * tail;
};

/*initialize a double linked list node*/
void init_dl_list_node(dl_list_node * new_node, int cached_key_1, int cached_value_1, int cached_pkt_1, dl_list_head * list_head_p_temp);

/*insert a node to the head of the dll(double linked list)*/
void insert_node_head(dl_list_head * list_head, dl_list_node * new_node);

/*delete a node which at the tail of a dll*/
int delete_node_tail(dl_list_head * list_head);

/*delete a node in a dll with this node's pointer given*/
void delete_node_pointed(dl_list_head * list_head_p, dl_list_node * list_ptr);

/*insert a node at the top of the dll*/
void insert_data_before(dl_list_node * old_node, dl_list_node * new_node);





#endif // DLLIST_H_INCLUDED
