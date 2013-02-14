#ifndef LLIST_HEADER
#define LLIST_HEADER

#include <linux/kernel.h>  /* printk() */

typedef struct node_
{
  pid_t pid;
  // Treating these as bools
  unsigned read_lock;
  unsigned write_lock;

  struct node_ *next;
} node;

typedef node* node_t;

node_t check_in_list (node_t head, pid_t pid);

int insert_node (node_t *head, pid_t pid, unsigned read_lock,
			unsigned write_lock);

void remove_node (node_t *head, pid_t pid);

// Check if pid is in the list. Returns null if not
node_t check_in_list (node_t head, pid_t pid)
{
  node_t traversal = head;
 
  while(traversal != NULL) {
    if(traversal->pid == pid) {
      return traversal;
    }

    traversal = traversal->next;
  }

  return NULL;
}

// Insert a node into the list. Returns zero on success, nonzero on failure
int
insert_node (node_t *head, pid_t pid, unsigned read_lock,
        unsigned write_lock)
{
  node_t insert, traversal;
  if(!(*head))
  {
    (*head) = (node_t) kmalloc (sizeof (node), GFP_ATOMIC);
    (*head)->pid = pid;
    (*head)->read_lock = read_lock;
    (*head)->write_lock = write_lock;
    (*head)->next = NULL;
    return 0;
  }

  if(head == NULL) {
    return 1;
  }

  traversal = *head;
  
  while(traversal->next != NULL) {
    if(traversal->pid == pid) {
      return 1;
    }
    traversal = traversal->next; 
  }
  // Need to check the last element
  if(traversal->pid == pid) {
    return 1;
  }

  insert = (node_t) kmalloc (sizeof (node), GFP_ATOMIC);
  insert->pid = pid;
  insert->read_lock = read_lock;
  insert->write_lock = write_lock;
  insert->next = NULL;

  traversal->next = insert;


  return 0;
}

// Remove specified file pointer
void
remove_node (node_t *head, pid_t pid)
{
  if(!(*head)) {
    return;
  }
  if((*head)->pid == pid) {
    node_t oldhead = *head;
    *head = oldhead->next;
    oldhead->pid = 0;
    oldhead->next = NULL;
    kfree (oldhead);
  }
  else {
    node_t deletion;
    node_t traversal = *head;

    while(traversal->next != NULL) {
      if(traversal->next->pid == pid) {
        break;
      }
      traversal = traversal->next;
    }

    if((traversal->next && traversal->next->pid != pid) ||
        (!traversal->next && traversal->pid != pid)) {
      printk("Not in this list...\n");
      return;
    }

    deletion = traversal->next;
    traversal->next = deletion->next;
    deletion->pid = 0;
    deletion->next = NULL;
    kfree (deletion);
  }
}


#endif
