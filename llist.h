#ifndef LLIST_HEADER
#define LLIST_HEADER

#include <linux/kernel.h>  /* printk() */

typedef struct node_
{
  struct file *filp;
  // Treating these as bools
  unsigned read_lock;
  unsigned write_lock;

  struct node_ *next;
} node;

typedef node* node_t;

node_t check_in_list (node_t head, struct file *filp);

int insert_node (node_t *head, struct file *filp, unsigned read_lock,
			unsigned write_lock);

void remove_node (node_t *head, struct file *filp);

// Check if filp is in the list. Returns null if not
node_t check_in_list (node_t head, struct file *filp)
{
  node_t traversal = head;
 
  while(traversal != NULL) {
    if(traversal->filp == filp) {
      return traversal;
    }

    traversal = traversal->next;
  }

  return NULL;
}

// Insert a node into the list. Returns zero on success, nonzero on failure
int
insert_node (node_t *head, struct file *filp, unsigned read_lock,
        unsigned write_lock)
{
  if(!(*head))
  {
    (*head) = (node_t) kmalloc (sizeof (node), GFP_ATOMIC);
    (*head)->filp = filp;
    (*head)->read_lock = read_lock;
    (*head)->write_lock = write_lock;
    (*head)->next = NULL;
    return 0;
  }
  node_t insert, traversal;

  if(head == NULL) {
    return 1;
  }

  traversal = *head;
  
  while(traversal->next != NULL) {
    if(traversal->filp == filp) {
      return 1;
    }
    traversal = traversal->next; 
  }
  // Need to check the last element
  if(traversal->filp == filp) {
    return 1;
  }

  insert = (node_t) kmalloc (sizeof (node), GFP_ATOMIC);
  insert->filp = filp;
  insert->read_lock = read_lock;
  insert->write_lock = write_lock;
  insert->next = NULL;

  traversal->next = insert;


  return 0;
}

// Remove specified file pointer
void
remove_node (node_t *head, struct file *filp)
{
  if(!(*head)) {
    return;
  }
  if((*head)->filp == filp) {
    node_t oldhead = *head;
    *head = oldhead->next;
    oldhead->filp = NULL;
    oldhead->next = NULL;
    kfree (oldhead);
  }
  else {
    node_t deletion;
    node_t traversal = *head;

    while(traversal->next != NULL) {
      if(traversal->next->filp == filp) {
        break;
      }
      traversal = traversal->next;
    }

    if((traversal->next && traversal->next->filp != filp) ||
        (!traversal->next && traversal->filp != filp)) {
      printk("Not in this list...\n");
      return;
    }

    deletion = traversal->next;
    traversal->next = deletion->next;
    deletion->filp = NULL;
    deletion->next = NULL;
    kfree (deletion);
  }
}


#endif
