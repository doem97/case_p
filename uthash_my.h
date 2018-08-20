#ifndef UTHASH_MY_H_INCLUDED
#define UTHASH_MY_H_INCLUDED

#include "allmyfile.h"

/*Defining a structure that can be hashed*/
struct packet
{
    int id;                    /* key */
    dl_list_node * hash_to_list;
    UT_hash_handle hh;         /* makes this structure hashable */
};

//declaration of hash_table
extern struct packet * hash_table;

/*add user, must make sure key is not used before adding*/
void add_user(int user_id, dl_list_node * node_pointer);

/*Find a structure using its key,return pointer when found, return NULL when not found*/
struct packet * find_user(int user_id);

/*Delete an item from a hash*/
void delete_user(struct packet *user);

/*Delete all items from a hash*/
void delete_all();

/*Count of items in the hash table,return 0 if NULL*/
unsigned int count_table();

#endif // UTHASH_MY_H_INCLUDED
