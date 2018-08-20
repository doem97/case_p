#include "allmyfile.h"

struct packet * hash_table = NULL;    /* important! initialize to NULL */

/*add user, must make sure key is not used before adding*/
void add_user(int user_id, dl_list_node * node_pointer)
{
    struct packet *s;
    s = (struct packet *)malloc(sizeof(struct packet));
    s->id = user_id;
    s->hash_to_list = node_pointer;
    HASH_ADD_INT( hash_table, id, s );  /* id: name of key field */
    return;
}

/*Find a structure using its key,return pointer when found, return NULL when not found*/
struct packet * find_user(int user_id)
{
    struct packet *s;
    HASH_FIND_INT( hash_table, &user_id, s );  /* s: output pointer */
    return s;
}

/*Delete an item from a hash*/
void delete_user(struct packet *user)
{
    HASH_DEL( hash_table, user);  /* user: pointer to deletee */
    free(user);              /* optional; it's up to you! */
    return;
}

/*Delete all items from a hash*/
void delete_all()
{
  struct packet *current_user, *tmp;

  HASH_ITER(hh, hash_table, current_user, tmp)
  {
    HASH_DEL( hash_table,current_user);  /* delete; users advances to next */
    free(current_user);            /* optional- if you want to free  */
  }
  return;
}

/*Count of items in the hash table,return 0 if NULL*/
unsigned int count_table()
{
    unsigned int num_users;
    num_users = HASH_COUNT(hash_table);
    return num_users;
}

