#include "llist.h"
#include "stdlib.h"
#include "stdio.h"


//Initialize the Linked List
node_t
initialize_llist (struct file *filp, unsigned read_lock, unsigned write_lock)
{
  node_t head = (node_t) malloc(sizeof(node)); // For actual use: kmalloc (sizeof (node), GFP_ATOMIC);
  head->filp = filp;
  head->read_lock = read_lock;
  head->write_lock = write_lock;
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

// Insert a node into the list. Returns zero on success, nonzero on failure
int
insert_node (node_t head, struct file *filp, unsigned read_lock,
        unsigned write_lock)
{
  node_t insert;

  if(head == NULL) {
    return 1;
  }

  node_t traversal = head;
  
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

  insert = (node_t) malloc(sizeof(node)); // For actual use: kmalloc (sizeof (node), GFP_ATOMIC);
  insert->filp = filp;
  insert->read_lock = read_lock;
  insert->write_lock = write_lock;
  insert->next = NULL;

  traversal->next = insert;


  return 0;
}

// Remove specified file pointer
void
remove_node (node_t head, struct file *filp)
{
  if(head->filp == filp) {
    node_t oldhead = head;
    head = oldhead->next;
    oldhead->filp = NULL;
    oldhead->next = NULL;
    free (oldhead); //Acutal use: kfree (oldhead);
  }
  else {
    node_t traversal = head;

    while(traversal->next != NULL) {
      if(traversal->next->filp == filp) {
        break;
      }
      traversal = traversal->next;
    }

    if((traversal->next && traversal->next->filp != filp) ||
        (!traversal->next && traversal->filp != filp))
      return;

    node_t deletion = traversal->next;
    traversal->next = deletion->next;
    deletion->filp = NULL;
    deletion->next = NULL;
    free (deletion); //Acutal use: kfree (deletion);
  }
}

void
print_llist(node_t n)
{
  if(!n)
    return;
  printf("List:\n");
  do
  {
    printf("file: %d, write: %d, read: %d\n", (int)n->filp, (int)n->write_lock, (int)n->read_lock);
    n = n->next;
  }
  while(n != NULL);
}

/* Testing
int
main(void)
{
  int test = 0;
  node_t head = initialize_llist((struct file *)1, 0, 1);
  
  test = insert_node(head, (struct file *)2, 1, 0);
  if(test) {
    printf("Exiting cannot insert\n");
    return 1;
  }
  test = insert_node(head, (struct file *)3, 0, 1);
  if(test) {
    printf("Exiting cannot insert\n");
    return 1;
  }
  print_llist(head);

  remove_node (head, (struct file *)2);

  print_llist(head);

  test = insert_node(head, (struct file *)3, 0, 1);
  if(test == 0) {
    printf("Test failed ... inserted node that shouldn't have inerted\n");
  }

  print_llist(head);

  remove_node (head, (struct file *)2);

  print_llist(head);


  remove_node (head, (struct file *)3);

  print_llist(head);
}
*/


