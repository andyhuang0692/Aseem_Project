#include "llist.h"
#include "alloc.h"
#include "stdlib.h"

//Initialize the Linked List
node_t
initialize_llist ()
{
  node_t head = kmalloc (sizeof (node), GFP_ATOMIC);
  head->filp = NULL;
  head->read_lock = false;
  head->write_lock = false;
  head->next = NULL;

  return head;
}

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

void
insert_node (node_t head, struct file *filp, unsigned read_lock,
        unsigned write_lock)
{
  if(head == NULL) {
    head = initialize_llist ();
    head->read_lock = read_lock;
    head->write_lock = write_lock;
    head->next = NULL;
  }
  else if(check_in_list(head, filp) != NULL) {
    eprintk("Cannot Add into Queue because it will because a deadlock!");
  }
  else {
    node_t insert = kmalloc (sizeof (node), GFP_ATOMIC);
    insert->filp = filp;
    insert->read_lock = read_lock;
    insert->write_lock = write_lock;
    insert->next = NULL;

    node_t traversal = head;
    
    while(traversal->next != NULL) 
      traversal = traversal->next; 

    traversal->next = insert;
  }
  
}

void
remove_node (node_t head, struct file *filp)
{
  if(head->filp == filp) {
    node_t oldhead = head;
    head = oldhead->next;
    oldhead->filp = NULL;
    oldhead->next = NULL;
    kfree (oldhead);
  }
  else if(check_in_list(head, filp) == NULL) {
    eprintk("Task is not currently in the List");
  }
  else {
    node_t traversal = head;

    while(traversal->next != NULL) {
      if(traversal->next->filp == filp) {
        break;
      }
      traversal = traversal->next;
    }

    node_t deletion = traversal->next;
    traversal->next = deletion->next;
    deletion->filp = NULL;
    deletion->next = NULL;
    kfree (deletion);
  }
}
