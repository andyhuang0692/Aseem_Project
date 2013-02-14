//#include <linux/file.h>

#ifndef LLIST_HEADER
#define LLIST_HEADER

typedef struct node_
{
  struct file *filp;
  // Treating these as bools
  unsigned read_lock;
  unsigned write_lock;

  struct node_ *next;
} node;

typedef node* node_t;

node_t initialize_llist (struct file *filp, unsigned read_lock, unsigned write_lock);

node_t check_in_list (node_t head, struct file *filp);

int insert_node (node_t head, struct file *filp, unsigned read_lock,
			unsigned write_lock);

void remove_node (node_t head, struct file *filp);

#endif
